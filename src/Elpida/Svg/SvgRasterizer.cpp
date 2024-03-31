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
			finalBackDrop.Draw(rasterizedSelf.GetBackdrop(), rasterizedSelf.GetX() - minX,rasterizedSelf.GetY() - minY);
		}

		for (std::size_t i = 0; i < rasterizedChildren.size(); ++i)
		{
			auto& rasterizedChild = rasterizedChildren[i];
			auto& child = children[i];

			finalBackDrop.Draw(rasterizedChild.GetBackdrop(),
					rasterizedChild.GetX() - minX,
					rasterizedChild.GetY() - minY,
					child.GetOpacity(),
					child.BlendMode(),
					child.CompositingMode());
		}

		return RasterizedShape(std::move(finalBackDrop), minX, minY);
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
		if (shape.GetPaths().empty()) return {};
		return std::async([&, subSamples]()
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

		backDrop.Draw(polygon, paint, fillRule, SvgBlendMode::Normal, SvgCompositingMode::SourceOver, subSamples);

		return { std::move(backDrop), x, y };
	}
} // Elpida
