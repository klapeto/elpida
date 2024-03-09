//
// Created by klapeto on 5/3/2024.
//

#ifndef ELPIDA_SVG_SVGSHAPEPOLYGONIZER_HPP
#define ELPIDA_SVG_SVGSHAPEPOLYGONIZER_HPP

#include <vector>

#include "Elpida/Svg/SvgPolygon.hpp"

namespace Elpida
{
	class SvgElement;
	class SvgPathInstance;
	class SvgStroke;
	class SvgCalculatedShape;
	class SvgCalculationContext;

	class SvgShapePolygonizer
	{
	public:
		static SvgPolygon Polygonize(const SvgCalculatedShape& shape);
		static SvgPolygon PolygonizeStroke(const SvgCalculatedShape& shape, const SvgCalculationContext& calculationContext, const SvgStroke& stroke);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSHAPEPOLYGONIZER_HPP
