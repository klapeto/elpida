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
	static std::size_t Round(double x)
	{
		return std::round(x);
	}

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
				rasterizationFutures.push_back(std::async( [&]()
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

		auto& selfBounds = rasterizedSelf.GetActualBounds();

		SvgBounds bounds = SvgBounds::CreateMinimum();

		if (selfBounds.IsValid())
		{
			bounds.Merge(selfBounds);
		}

		for (auto& rasterizedChild: rasterizedChildren)
		{
			auto childBounds = rasterizedChild.GetActualBounds();
			if (childBounds.IsValid())
			{
				bounds.Merge(rasterizedChild.GetActualBounds());
			}
		}

		if (!bounds.IsValid()) return {};

		SvgBackDrop finalBackDrop(std::ceil(bounds.GetWidth()), std::ceil(bounds.GetHeight()));

		finalBackDrop.Draw(rasterizedSelf.GetBackdrop(),
				Round(selfBounds.GetMinX() - bounds.GetMinX()),
				Round(selfBounds.GetMinX() - bounds.GetMinX()));

		for (std::size_t i = 0; i < rasterizedChildren.size(); ++i)
		{
			auto& rasterized = rasterizedChildren[i];
			auto& child = children[i];

			auto& childBounds = rasterized.GetActualBounds();
			finalBackDrop.Draw(rasterized.GetBackdrop(),
					Round(std::max(0.0, childBounds.GetMinX() - std::max(0.0, bounds.GetMinX()))),
							Round(std::max(0.0, childBounds.GetMinY() - std::max(0.0, bounds.GetMinY()))),
					child.GetOpacity(),
					child.BlendMode(),
					child.CompositingMode());
		}

		return RasterizedShape(std::move(finalBackDrop), bounds);
	}

	void SvgRasterizer::RasterizeRootShape(SvgBackDrop& backDrop, SvgCalculatedShape& shape, std::size_t subSamples)
	{
		auto childRasterizedShape = RasterizeShape(shape, subSamples);
		auto& childBounds = childRasterizedShape.GetActualBounds();
		backDrop.Draw(childRasterizedShape.GetBackdrop(),
				Round(std::max(0.0, childBounds.GetMinX())),
				Round(std::max(0.0, childBounds.GetMinY())),
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

			SvgBounds totalBounds = SvgBounds::CreateMinimum();

			RasterizedShape fill;
			if (fillRasterization.valid())
			{
				fill = std::move(fillRasterization.get());
				totalBounds.Merge(fill.GetActualBounds());
			}

			RasterizedShape stroke;
			if (strokeRasterization.valid())
			{
				stroke = std::move(strokeRasterization.get());
				totalBounds.Merge(stroke.GetActualBounds());
			}

			if (!totalBounds.IsValid()) return RasterizedShape();

			SvgBackDrop finalBackdrop(std::ceil(totalBounds.GetWidth()), std::ceil(totalBounds.GetHeight()));

			auto& fillBounds = fill.GetActualBounds();
			auto& strokeBounds = stroke.GetActualBounds();

			finalBackdrop.Draw(fill.GetBackdrop(),
					Round(std::max(0.0, fillBounds.GetMinX() - std::max(0.0, totalBounds.GetMinX()))),
					Round(std::max(0.0, fillBounds.GetMinY() - std::max(0.0, totalBounds.GetMinY()))));
			finalBackdrop.Draw(stroke.GetBackdrop(),
					Round(std::max(0.0, strokeBounds.GetMinX() - std::max(0.0, totalBounds.GetMinX()))),
					Round(std::max(0.0, strokeBounds.GetMinY() - std::max(0.0, totalBounds.GetMinY()))));

			return RasterizedShape(std::move(finalBackdrop), totalBounds);
		});
	}

	SvgRasterizer::RasterizedShape
	SvgRasterizer::RasterizePolygon(SvgPolygon polygon, SvgCalculatedPaint& paint, SvgFillRule fillRule,
			std::size_t subSamples)
	{
		auto bounds = polygon.GetBounds();

		SvgTransform transform;
		transform.Translate(-std::max(0.0, bounds.GetMinX()),-std::max(0.0, bounds.GetMinY()));

		polygon.Transform(transform);
		paint.Transform(transform);

		SvgBackDrop backDrop(std::ceil(bounds.GetWidth()), std::ceil(bounds.GetHeight()));

		backDrop.Draw(polygon, paint, fillRule, SvgBlendMode::Normal, SvgCompositingMode::SourceOver, subSamples);

		return { std::move(backDrop), bounds };
	}
} // Elpida
