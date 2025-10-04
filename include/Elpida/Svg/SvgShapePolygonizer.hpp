/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
		static SvgPolygon PolygonizeStroke(const SvgCalculatedShape& shape);
	};
} // Elpida

#endif //ELPIDA_SVG_SVGSHAPEPOLYGONIZER_HPP
