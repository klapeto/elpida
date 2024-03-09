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

	SvgBackDrop SvgRasterizer::Rasterize(const SvgDocument& document, double scale, std::size_t subSamples)
	{
		auto& rootSvgElement = document.GetElement();
		auto& viewBox = rootSvgElement.GetViewBox();
		auto& viewPort = rootSvgElement.GetViewPort();

		SvgCalculationContext calculationContext(1.0, 96.0);

		SvgBackDrop backDrop(
				viewBox.IsValid() ? viewBox.GetWidth() : viewPort.GetWidth().CalculateValue(calculationContext, 300.0),
				viewBox.IsValid() ? viewBox.GetHeight() : viewPort.GetHeight().CalculateValue(calculationContext,
						150.0));

		RasterizeShape(backDrop, rootSvgElement.CalculateShape(document, calculationContext));

		return backDrop;
	}

	void SvgRasterizer::RasterizeShape(SvgBackDrop& backDrop, const SvgCalculatedShape& shape)
	{
		if (shape.GetFill().has_value())
		{
			auto polygon = SvgShapePolygonizer::Polygonize(shape);
			backDrop.Draw(polygon, shape.GetFill().value());
		}

		if (shape.GetStroke().has_value())
		{
			auto polygon = SvgShapePolygonizer::PolygonizeStroke(shape);
			backDrop.Draw(polygon, shape.GetStroke().value());
		}

		for (auto& child : shape.GetChildren())
		{
			RasterizeShape(backDrop, child);
		}
	}
} // Elpida
