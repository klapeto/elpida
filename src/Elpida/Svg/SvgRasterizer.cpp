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

#include <iostream>
#include <future>

namespace Elpida
{
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

		RasterizedShape(SvgBackDrop&& backDrop, std::size_t x, std::size_t y)
				:_backdrop(std::move(backDrop)), _x(x), _y(y)
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

	class MultiThreadPolygonRasterizer
	{
	public:
		explicit MultiThreadPolygonRasterizer() = default;

		void operator()(SvgBackDrop& backDrop,
				const SvgPolygon& polygon,
				SvgCalculatedPaint& paint,
				const SvgSuperSampler& superSampler,
				SvgFillRule fillRule)
		{
			backDrop.DrawMultiThread(polygon, paint, superSampler, fillRule, SvgBlendMode::Normal,
					SvgCompositingMode::SourceOver);
		}
	};

	class MultiThreadFutureGenerator
	{
	public:
		template<typename T>
		using FutureType = std::future<T>;

		MultiThreadFutureGenerator() = default;

		template<typename T, typename TCallable>
		FutureType<T> Generate(TCallable callable)
		{
			return std::async(callable);
		}
	};

	class MultiThreadBackDropDrawer
	{
	public:
		explicit MultiThreadBackDropDrawer() = default;

		void operator()(SvgBackDrop& backDrop,
				const SvgBackDrop& other,
				std::size_t x,
				std::size_t y,
				double opacity,
				SvgBlendMode blendMode,
				SvgCompositingMode compositingMode)
		{
			backDrop.DrawMultiThread(other, x, y, opacity, blendMode, compositingMode);
		}
	};

	template<typename T>
	class CompletedFuture
	{
	public:
		T& get()
		{
			return _value.value();
		}

		bool valid() const
		{
			return _value.has_value();
		}

		CompletedFuture() = default;

		explicit CompletedFuture(T&& value)
				:_value(std::move(value))
		{
		}

	private:
		std::optional<T> _value;
	};

	class SingleThreadPolygonRasterizer
	{
	public:

		void operator()(SvgBackDrop& backDrop,
				const SvgPolygon& polygon,
				SvgCalculatedPaint& paint,
				const SvgSuperSampler& superSampler,
				SvgFillRule fillRule)
		{
			backDrop.Draw(polygon, paint, superSampler, fillRule, SvgBlendMode::Normal, SvgCompositingMode::SourceOver);
		}
	};

	class SingleThreadFutureGenerator
	{
	public:
		template<typename T>
		using FutureType = CompletedFuture<T>;

		template<typename T, typename TCallable>
		FutureType<T> Generate(TCallable callable)
		{
			return CompletedFuture<T>(callable());
		}
	};

	class SingleThreadBackDropDrawer
	{
	public:
		void operator()(SvgBackDrop& backDrop,
				const SvgBackDrop& other,
				std::size_t x,
				std::size_t y,
				double opacity,
				SvgBlendMode blendMode,
				SvgCompositingMode compositingMode)
		{
			backDrop.Draw(other, x, y, opacity, blendMode, compositingMode);
		}
	};

	template<typename TPolyRasterize>
	static RasterizedShape
	RasterizePolygon(SvgPolygon polygon,
			SvgCalculatedPaint& paint,
			SvgFillRule fillRule,
			const SvgSuperSampler& superSampler,
			TPolyRasterize& rasterize)
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

		rasterize(backDrop, polygon, paint, superSampler, fillRule);

		return { std::move(backDrop), x, y };
	}

	template<typename TFuture, typename TFutureGenerator, typename TPolyRasterize, typename TBackdropRasterize>
	static TFuture
	RasterizedSelfShape(const SvgCalculatedShape& shape,
			const SvgSuperSampler& superSampler,
			TFutureGenerator& futureGenerator,
			TPolyRasterize& polyRasterize,
			TBackdropRasterize& backdropRasterize)
	{
		if (shape.GetPaths().empty()) return {};
		return futureGenerator.template Generate<RasterizedShape>([&]()
		{
			TFuture fillRasterization;
			TFuture strokeRasterization;

			if (shape.GetFill().has_value())
			{
				fillRasterization = futureGenerator.template Generate<RasterizedShape>([&]()
				{
					auto paint = shape.GetFill().value();
					return RasterizePolygon(shape.GetFillPolygon(), paint,
							shape.GetFill()->GetFillRule(), superSampler, polyRasterize);
				});
			}

			if (shape.GetStroke().has_value())
			{
				strokeRasterization = futureGenerator.template Generate<RasterizedShape>([&]()
				{
					auto paint = shape.GetStroke().value();
					return RasterizePolygon(shape.GetStrokePolygon(), paint,
							SvgFillRule::NonZero, superSampler, polyRasterize);
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

			backdropRasterize(finalBackdrop, fill.GetBackdrop(), fill.GetX() - x, fill.GetY() - y, 1.0,
					SvgBlendMode::Normal, SvgCompositingMode::SourceOver);
			backdropRasterize(finalBackdrop, stroke.GetBackdrop(), stroke.GetX() - x, stroke.GetY() - y, 1.0,
					SvgBlendMode::Normal, SvgCompositingMode::SourceOver);

			return RasterizedShape(std::move(finalBackdrop), x, y);
		});
	}

	template<typename TFutureGenerator, typename TPolyRasterize, typename TBackdropRasterize>
	static RasterizedShape
	RasterizeShape(const SvgCalculatedShape& shape,
			const SvgSuperSampler& superSampler,
			TFutureGenerator& futureGenerator,
			TPolyRasterize& polyRasterize,
			TBackdropRasterize& backdropRasterize)
	{
		using TFuture = typename TFutureGenerator::template FutureType<RasterizedShape>;
		auto selfDrawFuture = RasterizedSelfShape<TFuture>(shape, superSampler, futureGenerator, polyRasterize,
				backdropRasterize);

		std::vector<RasterizedShape> rasterizedChildren;
		auto& children = shape.GetChildren();
		rasterizedChildren.reserve(children.size());
		{
			std::vector<TFuture> rasterizationFutures;

			rasterizationFutures.reserve(children.size());
			for (auto& child : children)
			{
				rasterizationFutures.push_back(futureGenerator.template Generate<RasterizedShape>([&]()
				{
					return RasterizeShape(child, superSampler, futureGenerator, polyRasterize, backdropRasterize);
				}));
			}

			for (auto& future : rasterizationFutures)
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

		for (auto& rasterizedChild : rasterizedChildren)
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
			backdropRasterize(finalBackDrop,
					rasterizedSelf.GetBackdrop(),
					rasterizedSelf.GetX() - minX,
					rasterizedSelf.GetY() - minY,
					1.0,
					SvgBlendMode::Normal,
					SvgCompositingMode::SourceOver);
		}

		for (std::size_t i = 0; i < rasterizedChildren.size(); ++i)
		{
			auto& rasterizedChild = rasterizedChildren[i];
			auto& child = children[i];

			backdropRasterize(finalBackDrop,
					rasterizedChild.GetBackdrop(),
					rasterizedChild.GetX() - minX,
					rasterizedChild.GetY() - minY,
					child.GetOpacity(),
					child.BlendMode(),
					child.CompositingMode());
		}

		return RasterizedShape(std::move(finalBackDrop), minX, minY);
	}

	template<typename TFutureGenerator, typename TPolyRasterize, typename TBackdropRasterize>
	static void RasterizeRootShape(SvgBackDrop& backDrop,
			const SvgCalculatedShape& shape,
			const SvgSuperSampler& superSampler,
			TFutureGenerator futureGenerator,
			TPolyRasterize polyRasterize,
			TBackdropRasterize backdropRasterize)
	{
		auto childRasterizedShape = RasterizeShape(shape,
				superSampler,
				futureGenerator,
				polyRasterize,
				backdropRasterize);
		backdropRasterize(backDrop, childRasterizedShape.GetBackdrop(),
				childRasterizedShape.GetX(),
				childRasterizedShape.GetY(),
				shape.GetOpacity(),
				shape.BlendMode(),
				shape.CompositingMode());
	}

	SvgBackDrop SvgRasterizer::Rasterize(const SvgCalculatedDocument& document, const SvgSuperSampler& superSampler,
			bool multiThread)
	{
		auto& viewPort = document.GetViewPort();
		SvgBackDrop backDrop(std::ceil(viewPort.GetWidth()), std::ceil(viewPort.GetHeight()));

		if (multiThread)
		{
			RasterizeRootShape(backDrop,
					document.GetRootShape(),
					superSampler,
					MultiThreadFutureGenerator(),
					MultiThreadPolygonRasterizer(),
					MultiThreadBackDropDrawer());
		}
		else
		{
			RasterizeRootShape(backDrop,
					document.GetRootShape(),
					superSampler,
					SingleThreadFutureGenerator(),
					SingleThreadPolygonRasterizer(),
					SingleThreadBackDropDrawer());
		}
		
		return backDrop;
	}

} // Elpida
