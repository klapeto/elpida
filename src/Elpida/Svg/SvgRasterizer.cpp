//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgCalculatedPaint.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Svg/SvgShapePolygonizer.hpp"
#include "Elpida/Svg/SvgViewBox.hpp"
#include "Elpida/Svg/SvgViewPort.hpp"

#include <iostream>
#include <future>

namespace Elpida
{
	static SvgTransform CalculateTransform(const SvgCalculatedViewPort& viewPort, const SvgViewBox& viewBox,
			const SvgPreserveAspectRatio& preserveAspectRatio)
	{
		if (!viewBox.IsValid()) return {};
		auto vbX = viewBox.GetMinX();
		auto vbY = viewBox.GetMinY();
		auto vbWidth = viewBox.GetWidth();
		auto vbHeight = viewBox.GetHeight();

		auto eX = viewPort.GetX();
		auto eY = viewPort.GetY();
		auto eWidth = viewPort.GetWidth();
		auto eHeight = viewPort.GetHeight();

		auto alignX = preserveAspectRatio.GetAlignX();
		auto alignY = preserveAspectRatio.GetAlignY();
		auto meetOrSlice = preserveAspectRatio.GetType();

		auto scaleX = eWidth / vbWidth;
		auto scaleY = eHeight / vbHeight;

		if (alignX != SvgAxisAlignType::None && meetOrSlice == SvgAlignType::Meet)
		{
			if (scaleX > scaleY)
			{
				scaleX = scaleY;
			}
			else
			{
				scaleY = scaleX;
			}
		}
		else if (alignX != SvgAxisAlignType::None && meetOrSlice == SvgAlignType::Slice)
		{
			if (scaleX < scaleY)
			{
				scaleX = scaleY;
			}
			else
			{
				scaleY = scaleX;
			}
		}

		auto translateX = eX - (vbX * scaleX);
		auto translateY = eY - (vbY * scaleY);

		if (alignX == SvgAxisAlignType::Mid)
		{
			translateX += (eWidth - vbWidth * scaleX) / 2.0;
		}
		else if (alignX == SvgAxisAlignType::Max)
		{
			translateX += (eWidth - vbWidth * scaleX);
		}

		if (alignY == SvgAxisAlignType::Mid)
		{
			translateY += (eHeight - vbHeight * scaleY) / 2.0;
		}
		else if (alignY == SvgAxisAlignType::Max)
		{
			translateY += (eHeight - vbHeight * scaleY);
		}

		SvgTransform transform;
		transform.Translate(translateX, translateY)
				.Scale(scaleX, scaleY);

		return transform;
	}

	SvgBackDrop SvgRasterizer::Rasterize(const SvgDocument& document, double scale, std::size_t subSamples)
	{
		auto& rootSvgElement = document.GetElement();
		auto& viewBox = rootSvgElement.GetViewBox();
		auto& viewPort = rootSvgElement.GetViewPort();

		SvgCalculationContext calculationContext(1.0, 96.0);

		auto calculatedViewPort = SvgCalculatedViewPort(
				0,
				0,
				viewPort.GetWidth().CalculateValue(calculationContext, 300.0),
				viewPort.GetHeight().CalculateValue(calculationContext, 150.0));

		auto calculatedViewBox = viewBox.IsValid()
								 ? viewBox
								 : SvgViewBox(0, 0, calculatedViewPort.GetWidth(), calculatedViewPort.GetHeight());

		SvgBackDrop backDrop(calculatedViewPort.GetWidth() * scale, calculatedViewPort.GetHeight() * scale);

		auto calculated = rootSvgElement.CalculateShape(document, calculationContext);

		auto transform = CalculateTransform(calculatedViewPort, calculatedViewBox,
				rootSvgElement.GetPreserveAspectRatio());

		transform.Scale(scale, scale);

		calculated.Transform(transform);

		RasterizeRootShape(backDrop, calculated, subSamples);

		return backDrop;
	}

	SvgRasterizer::RasterizedShape SvgRasterizer::RasterizeShape(SvgCalculatedShape& shape, std::size_t subSamples)
	{
		auto selfDrawFuture = RasterizedSelfShape(shape, subSamples);

		std::vector<RasterizedShape> rasterizedChildren;
		auto& children = shape.GetChildren();
		rasterizedChildren.reserve(children.size());
		{
			std::vector<std::future<RasterizedShape>> rasterizationFutures;

			rasterizationFutures.reserve(children.size());
			for (auto& child: children)
			{
				rasterizationFutures.push_back(std::async([&]()
				{
					return RasterizeShape(child, subSamples);
				}));
			}

			for (auto& future: rasterizationFutures)
			{
				rasterizedChildren.push_back(std::move(future.get()));
			}
		}

		auto rasterizedSelf = std::move(selfDrawFuture.get());


		std::size_t x = std::numeric_limits<std::size_t>::max();
		std::size_t y = std::numeric_limits<std::size_t>::max();
		std::size_t width = 0;
		std::size_t height = 0;

		if (rasterizedSelf.GetBackdrop().GetWidth() > 0)
		{
			x = std::min(x, rasterizedSelf.GetX());
			y = std::min(y, rasterizedSelf.GetY());
			width = std::max(width, rasterizedSelf.GetX() + rasterizedSelf.GetBackdrop().GetWidth());
			height = std::max(height, rasterizedSelf.GetY() + rasterizedSelf.GetBackdrop().GetHeight());
		}

		for (auto& rasterizedChild: rasterizedChildren)
		{
			x = std::min(x, rasterizedChild.GetX());
			y = std::min(y, rasterizedChild.GetY());
			width = std::max(width, rasterizedChild.GetX() + rasterizedChild.GetBackdrop().GetWidth());
			height = std::max(height, rasterizedChild.GetY() + rasterizedChild.GetBackdrop().GetHeight());
		}

		if (width == 0 || height == 0) return {};

		SvgBackDrop finalBackDrop(width, height);

		finalBackDrop.Draw(rasterizedSelf.GetBackdrop(), rasterizedSelf.GetX() - x, rasterizedSelf.GetY() - y);

		for (std::size_t i = 0; i < rasterizedChildren.size(); ++i)
		{
			auto& rasterized = rasterizedChildren[i];
			auto& child = children[i];

			finalBackDrop.Draw(rasterized.GetBackdrop(),
					rasterized.GetX() - x,
					rasterized.GetY() - y,
					child.GetOpacity(),
					child.BlendMode(),
					child.CompositingMode());
		}

		return RasterizedShape(std::move(finalBackDrop), x, y);
	}

	void SvgRasterizer::RasterizeRootShape(SvgBackDrop& backDrop, SvgCalculatedShape& shape, std::size_t subSamples)
	{
		auto childRasterizedShape = RasterizeShape(shape, subSamples);
		backDrop.Draw(childRasterizedShape.GetBackdrop(),
				childRasterizedShape.GetX(),
				childRasterizedShape.GetY(),
				shape.GetOpacity(),
				shape.BlendMode(),
				shape.CompositingMode());
	}

	std::future<SvgRasterizer::RasterizedShape>
	SvgRasterizer::RasterizedSelfShape(SvgCalculatedShape& shape, std::size_t subSamples)
	{
		return std::async(std::launch::deferred, [&, subSamples]()
		{
			std::future<RasterizedShape> fillRasterization;
			std::future<RasterizedShape> strokeRasterization;

			if (shape.GetFill().has_value())
			{
				fillRasterization = std::async([&]()
				{
					return RasterizePolygon(SvgShapePolygonizer::Polygonize(shape), shape.GetFill().value(),
							shape.GetFill()->GetFillRule(), subSamples);
				});
			}

			if (shape.GetStroke().has_value())
			{
				strokeRasterization = std::async([&]()
				{
					return RasterizePolygon(SvgShapePolygonizer::PolygonizeStroke(shape), shape.GetStroke().value(),
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
				width = std::max(width, fill.GetX() + fill.GetBackdrop().GetWidth());
				height = std::max(height, fill.GetY() + fill.GetBackdrop().GetHeight());
			}

			RasterizedShape stroke;
			if (strokeRasterization.valid())
			{
				stroke = std::move(strokeRasterization.get());
				x = std::min(x, stroke.GetX());
				y = std::min(y, stroke.GetY());
				width = std::max(width, stroke.GetX() + stroke.GetBackdrop().GetWidth());
				height = std::max(height, stroke.GetY() + stroke.GetBackdrop().GetHeight());
			}

			if (width == 0 || height == 0) return RasterizedShape();

			SvgBackDrop finalBackdrop(width, height);

			finalBackdrop.Draw(fill.GetBackdrop(), fill.GetX() - x, fill.GetY() - y);
			finalBackdrop.Draw(stroke.GetBackdrop(), stroke.GetX() - x, stroke.GetY() - y);

			return RasterizedShape(std::move(finalBackdrop), x, y);
		});
	}

	SvgRasterizer::RasterizedShape
	SvgRasterizer::RasterizePolygon(SvgPolygon polygon, SvgCalculatedPaint& paint, SvgFillRule fillRule,
			std::size_t subSamples)
	{

		auto bounds = polygon.GetBounds();

		auto gapX = bounds.GetMinX() > 0.0 ? 5 : 0;
		auto gapY = bounds.GetMinY() > 0.0 ? 5 : 0;

		auto offsetX = std::floor(std::max(0.0, bounds.GetMinX()));
		auto offsetY = std::floor(std::max(0.0, bounds.GetMinY()));

		std::size_t x = std::max(0.0, offsetX - gapX);
		std::size_t y = std::max(0.0, offsetY - gapY);

		SvgTransform transform;
		transform.Translate(-(static_cast<double>(offsetX)) + gapX, -(static_cast<double>(offsetY)) + gapY);

		polygon.Transform(transform);
		paint.Transform(transform);

		SvgBackDrop backDrop(std::ceil(bounds.GetWidth() + gapX * 2), std::ceil(bounds.GetHeight() + gapY * 2));

		backDrop.Draw(polygon, paint, fillRule, SvgBlendMode::Normal, SvgCompositingMode::SourceOver, subSamples);

		return { std::move(backDrop), x, y };
	}
} // Elpida
