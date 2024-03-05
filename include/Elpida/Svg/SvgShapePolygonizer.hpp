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

	class SvgShapePolygonizer
	{
	public:
		static SvgPolygon Polygonize(const std::vector<SvgPathInstance>& paths, double scale);
		static SvgPolygon PolygonizeStroke(const std::vector<SvgPathInstance>& paths, const SvgStroke& stroke, double scale);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSHAPEPOLYGONIZER_HPP
