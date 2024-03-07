//
// Created by klapeto on 24/12/2023.
//

#include "Elpida/Svg/SvgRasterizer.hpp"

#include <algorithm>
#include <iostream>
#include <Elpida/Svg/SvgDocument.hpp>
#include <Elpida/Svg/SvgGradient.hpp>
#include <Elpida/Svg/SvgPaint.hpp>
#include <Elpida/Svg/SvgEdge.hpp>

#include <cstring>
#include <list>
#include <memory>
#include <Elpida/Svg/SvgRasterizerPaint.hpp>
#include <Elpida/Svg/SvgShapePolygonizer.hpp>

namespace Elpida
{
	SvgBackDrop SvgRasterizer::Rasterize(const SvgDocument& document,double scale,std::size_t subSamples)
	{
		SvgBackDrop backDrop(document.GetActualWidth(), document.GetActualHeight());

		for (auto& element : document.GetElement().GetChildren())
		{
			if (!element.IsVisible()) continue;
			if (element.GetFill().IsSet())
			{
				auto polygon = SvgShapePolygonizer::Polygonize(element.GetPaths(), scale);

				auto paint = SvgRasterizerPaint(element.GetFill(), element, document);
				backDrop.Draw(polygon, paint, element.GetFill().GetFillRule());
			}

			if (element.GetStroke().IsSet())
			{
				auto polygon = SvgShapePolygonizer::PolygonizeStroke(element.GetPaths(),element.GetStroke(), scale);

				auto paint = SvgRasterizerPaint(element.GetStroke(), element, document);
				backDrop.Draw(polygon, paint);
			}
		}

		return backDrop;
	}
} // Elpida
