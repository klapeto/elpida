//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"
#include "Elpida/Svg/SvgShapePolygonizer.hpp"
#include "Elpida/Svg/SvgViewBox.hpp"
#include "Elpida/Svg/SvgViewPort.hpp"
#include "Elpida/Core/ThreadPool.hpp"

#include <iostream>
#include <future>

namespace Elpida
{
	class AsyncThreadPool
	{
	public:
		template<typename T, typename TCallable>
		std::future<T> Queue(TCallable&& callable)
		{
			return std::async(callable);
		}
	};

	class RasterizedShape final
	{
	public:
		[[nodiscard]]
		const SvgBackDrop& GetBackdrop() const
		{
			return _backdrop;
		}

		[[nodiscard]]
		std::size_t GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		std::size_t GetY() const
		{
			return _y;
		}

		[[nodiscard]]
		bool IsValid() const
		{
			return _backdrop.GetWidth() > 0;
		}

		RasterizedShape() = default;
		RasterizedShape(SvgBackDrop&& backDrop,  std::size_t x, std::size_t y)
				: _backdrop(std::move(backDrop)), _x(x), _y(y)
		{

		}

		RasterizedShape(const RasterizedShape&) noexcept = delete;
		RasterizedShape& operator=(const RasterizedShape&) = delete;
		RasterizedShape(RasterizedShape&&) noexcept = default;
		RasterizedShape& operator=(RasterizedShape&&) noexcept = default;
	private:
		SvgBackDrop _backdrop;
		std::size_t _x = 0;
		std::size_t _y = 0;
	};

	static RasterizedShape
	RasterizePolygon(SvgPolygon polygon,
			SvgCalculatedPaint& paint,
			ThreadPool& threadPool,
			SvgFillRule fillRule,
			std::size_t subSamples)
	{
		auto bounds = polygon.GetBounds();

		auto offsetX = std::floor(std::max(0.0, bounds.GetMinX()));
		auto offsetY = std::floor(std::max(0.0, bounds.GetMinY()));
		std::size_t x = offsetX;
		std::size_t y = offsetY;

		SvgTransform transform;
		transform.Translate(-(static_cast<double>(offsetX)), -(static_cast<double>(offsetY)));

		polygon.Transform(transform);
		paint.Transform(transform);

		// +0.5 because backdrop pixels are actually drawn at + 0.5 due to 0.0 is at the center of the first pixel
		SvgBackDrop backDrop(std::ceil(bounds.GetWidth() + 0.5), std::ceil(bounds.GetHeight() + 0.5));

		backDrop.DrawMultiThread(polygon, paint,threadPool, fillRule, SvgBlendMode::Normal, SvgCompositingMode::SourceOver, subSamples);

		return { std::move(backDrop), x, y };
	}

	static std::future<RasterizedShape> RasterizedSelfShape(SvgCalculatedShape& shape, ThreadPool& threadPool, std::size_t subSamples)
	{
		if (shape.GetPaths().empty()) return {};
		return threadPool.template Queue<RasterizedShape>([&, subSamples]()
		{
			std::future<RasterizedShape> fillRasterization;
			std::future<RasterizedShape> strokeRasterization;

			if (shape.GetFill().has_value())
			{
				fillRasterization = threadPool.template Queue<RasterizedShape>([&]()
				{
					return RasterizePolygon(SvgShapePolygonizer::Polygonize(shape), shape.GetFill().value(), threadPool,
							shape.GetFill()->GetFillRule(), subSamples);
				});
			}

			if (shape.GetStroke().has_value())
			{
				strokeRasterization = threadPool.template Queue<RasterizedShape>([&]()
				{
					return RasterizePolygon(SvgShapePolygonizer::PolygonizeStroke(shape), shape.GetStroke().value(), threadPool,
							SvgFillRule::NonZero, subSamples);
				});
			}
			std::size_t x = std::numeric_limits<std::size_t>::max();
			std::size_t y = std::numeric_limits<std::size_t>::max();
			std::size_t width = 0;
			std::size_t height = 0;

			RasterizedShape fill;
			if (fillRasterization.valid())
			{
				fill = std::move(fillRasterization.get());
				x = std::min(x, fill.GetX());
				y = std::min(y, fill.GetY());
				width = std::max(width, (fill.GetX() + fill.GetBackdrop().GetWidth()) - fill.GetX());
				height = std::max(height, (fill.GetY() + fill.GetBackdrop().GetHeight()) - fill.GetY());
			}

			RasterizedShape stroke;
			if (strokeRasterization.valid())
			{
				stroke = std::move(strokeRasterization.get());
				x = std::min(x, stroke.GetX());
				y = std::min(y, stroke.GetY());
				width = std::max(width, (stroke.GetX() + stroke.GetBackdrop().GetWidth()) - stroke.GetX());
				height = std::max(height, (stroke.GetY() + stroke.GetBackdrop().GetHeight()) - stroke.GetY());
			}

			if (width == 0 || height == 0) return RasterizedShape();

			SvgBackDrop finalBackdrop(width, height);

			finalBackdrop.DrawMultiThread(fill.GetBackdrop(), fill.GetX() - x, fill.GetY() - y, threadPool);
			finalBackdrop.DrawMultiThread(stroke.GetBackdrop(), stroke.GetX() - x, stroke.GetY() - y, threadPool);

			return RasterizedShape(std::move(finalBackdrop), x, y);
		});
	}


	static RasterizedShape RasterizeShape(SvgCalculatedShape& shape, ThreadPool& threadPool, std::size_t subSamples)
	{
		auto selfDrawFuture = RasterizedSelfShape(shape, threadPool, subSamples);

		std::vector<RasterizedShape> rasterizedChildren;
		auto& children = shape.GetChildren();
		rasterizedChildren.reserve(children.size());
		{
			std::vector<std::future<RasterizedShape>> rasterizationFutures;

			rasterizationFutures.reserve(children.size());
			for (auto& child: children)
			{
				rasterizationFutures.push_back(threadPool.template Queue<RasterizedShape>([&]()
				{
					return RasterizeShape(child, threadPool, subSamples);
				}));
			}

			for (auto& future: rasterizationFutures)
			{
				rasterizedChildren.push_back(std::move(future.get()));
			}
		}

		RasterizedShape rasterizedSelf;

		if (selfDrawFuture.valid())
		{
			rasterizedSelf = std::move(selfDrawFuture.get());
		}

		std::size_t minX = std::numeric_limits<std::size_t>::max();
		std::size_t minY = std::numeric_limits<std::size_t>::max();
		std::size_t maxX = 0;
		std::size_t maxY = 0;

		if (rasterizedSelf.IsValid())
		{
			minX = std::min(minX, rasterizedSelf.GetX());
			minY = std::min(minY, rasterizedSelf.GetY());
			maxX = std::max(maxX, rasterizedSelf.GetX() + rasterizedSelf.GetBackdrop().GetWidth());
			maxY = std::max(maxY, rasterizedSelf.GetY() + rasterizedSelf.GetBackdrop().GetHeight());
		}

		for (auto& rasterizedChild: rasterizedChildren)
		{
			minX = std::min(minX, rasterizedChild.GetX());
			minY = std::min(minY, rasterizedChild.GetY());
			maxX = std::max(maxX, rasterizedChild.GetX() + rasterizedChild.GetBackdrop().GetWidth());
			maxY = std::max(maxY, rasterizedChild.GetY() + rasterizedChild.GetBackdrop().GetHeight());
		}

		auto width = maxX - minX;
		auto height = maxY - minY;

		if (width == 0 || height == 0) return {};

		SvgBackDrop finalBackDrop(width, height);

		if (rasterizedSelf.IsValid())
		{
			finalBackDrop.DrawMultiThread(rasterizedSelf.GetBackdrop(), rasterizedSelf.GetX() - minX,rasterizedSelf.GetY() - minY, threadPool);
		}

		for (std::size_t i = 0; i < rasterizedChildren.size(); ++i)
		{
			auto& rasterizedChild = rasterizedChildren[i];
			auto& child = children[i];

			finalBackDrop.DrawMultiThread(rasterizedChild.GetBackdrop(),
					rasterizedChild.GetX() - minX,
					rasterizedChild.GetY() - minY,
					threadPool,
					child.GetOpacity(),
					child.BlendMode(),
					child.CompositingMode());
		}

		return RasterizedShape(std::move(finalBackDrop), minX, minY);
	}


	static void RasterizeRootShape(SvgBackDrop& backDrop, SvgCalculatedShape& shape, ThreadPool& threadPool, std::size_t subSamples)
	{
		auto childRasterizedShape = RasterizeShape(shape, threadPool, subSamples);
		backDrop.DrawMultiThread(childRasterizedShape.GetBackdrop(),
				childRasterizedShape.GetX(),
				childRasterizedShape.GetY(),
				threadPool,
				shape.GetOpacity(),
				shape.BlendMode(),
				shape.CompositingMode());
	}

	SvgBackDrop SvgRasterizer::Rasterize(SvgCalculatedDocument& document, std::size_t subSamples)
	{
		auto& viewPort = document.GetViewPort();
		SvgBackDrop backDrop(std::ceil(viewPort.GetWidth()), std::ceil(viewPort.GetHeight()));

		ThreadPool pool({});
		RasterizeRootShape(backDrop, document.GetRootShape(), pool, subSamples);

		return backDrop;
	}

	SvgBackDrop SvgRasterizer::RasterizeMultiThreaded(SvgCalculatedDocument& document,
			ThreadPool& threadPool,
			std::size_t subSamples)
	{
		auto& viewPort = document.GetViewPort();
		SvgBackDrop backDrop(std::ceil(viewPort.GetWidth()), std::ceil(viewPort.GetHeight()));

		RasterizeRootShape(backDrop, document.GetRootShape(), threadPool, subSamples);

		return backDrop;
	}
} // Elpida
