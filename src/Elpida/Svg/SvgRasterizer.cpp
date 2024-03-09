//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgCalculationContext.hpp"

#include <algorithm>
#include <iostream>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgGradient.hpp>
#include <Elpida/Svg/SvgPaint.hpp>
#include <Elpida/Svg/SvgEdge.hpp>

#include <cstring>
#include <list>
#include <memory>
#include <Elpida/Svg/SvgCalculatedPaint.hpp>
#include <Elpida/Svg/SvgShapePolygonizer.hpp>

namespace Elpida
{
	SvgBackDrop SvgRasterizer::Rasterize(const SvgDocument& document, double scale, std::size_t subSamples)
	{
		SvgBackDrop backDrop(document.GetElement().GetViewBox().GetWidth(), document.GetElement().GetViewBox().GetHeight());

		SvgCalculationContext calculationContext(document.GetElement().GetViewBox(), 1.0, 96.0);
		for (auto& element : document.GetElement().GetChildren())
		{
			if (!element.IsVisible()) continue;

			auto shape = element.CalculateShape(document, calculationContext);
			if (shape.GetFill().has_value())
			{
				auto polygon = SvgShapePolygonizer::Polygonize(shape);

				backDrop.Draw(polygon, shape.GetFill().value(), element.GetFill().GetFillRule());
			}

			if (element.GetStroke().IsSet())
			{
//				auto polygon = SvgShapePolygonizer::PolygonizeStroke(element.GetPaths(),element.GetStroke(), scale);
//
//				auto paint = SvgCalculatedPaint(element.GetStroke(), element, document);
//				backDrop.Draw(polygon, paint);
			}
		}

		return backDrop;
	}
} // Elpida
