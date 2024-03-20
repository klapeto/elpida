//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"

#include <algorithm>
#include <iostream>
#include <cstring>
#include <list>
#include <memory>

#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgGradient.hpp>
#include <Elpida/Svg/SvgPaint.hpp>
#include <Elpida/Svg/SvgEdge.hpp>
#include <Elpida/Svg/SvgCalculatedPaint.hpp>
#include <Elpida/Svg/SvgShapePolygonizer.hpp>
#include <Elpida/Svg/SvgShapePolygonizer.hpp>
#include <Elpida/Svg/SvgViewPort.hpp>
#include <Elpida/Svg/SvgViewBox.hpp>

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
				viewPort.GetHeight().CalculateValue(calculationContext,150.0));

		auto calculatedViewBox = viewBox.IsValid() ?  viewBox : SvgViewBox(0,0 ,calculatedViewPort.GetWidth(), calculatedViewPort.GetHeight());

		SvgBackDrop backDrop(calculatedViewPort.GetWidth(), calculatedViewPort.GetHeight());

		auto calculated = rootSvgElement.CalculateShape(document, calculationContext);

		auto transform = CalculateTransform(calculatedViewPort, calculatedViewBox, rootSvgElement.GetPreserveAspectRatio());

		calculated.Transform(transform);

		RasterizeShape(backDrop, calculated, transform, subSamples);

		return backDrop;
	}

	void SvgRasterizer::RasterizeShape(SvgBackDrop& backDrop, const SvgCalculatedShape& shape, const SvgTransform& transform, std::size_t subSamples)
	{
		if (shape.GetFill().has_value())
		{
			auto polygon = SvgShapePolygonizer::Polygonize(shape);
			backDrop.Draw(polygon, shape.GetFill().value(), shape.GetFill()->GetFillRule(), SvgBlendMode::Normal, SvgCompositingMode::SourceOver, subSamples);
		}

		if (shape.GetStroke().has_value())
		{
			auto polygon = SvgShapePolygonizer::PolygonizeStroke(shape);
			backDrop.Draw(polygon, shape.GetStroke().value(), SvgFillRule::NonZero, SvgBlendMode::Normal, SvgCompositingMode::SourceOver, subSamples);
		}

		for (auto& child : shape.GetChildren())
		{
			RasterizeShape(backDrop, child, transform, subSamples);
		}
	}
} // Elpida
