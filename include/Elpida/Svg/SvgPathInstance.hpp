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
// Created by klapeto on 3/1/2024.
//

#ifndef ELPIDA_SVG_SVGPATHINSTANCE_HPP
#define ELPIDA_SVG_SVGPATHINSTANCE_HPP

#include <vector>

#include "Elpida/Svg/SvgBounds.hpp"
#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgCubicBezierCurve.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{

	class SvgPathInstance
	{
	public:
		[[nodiscard]]
		const std::vector<SvgCubicBezierCurve>& GetCurves() const
		{
			return _curves;
		}

		[[nodiscard]]
		const SvgBounds& GetBounds() const
		{
			return _bounds;
		}

		[[nodiscard]]
		bool IsClosed() const
		{
			return _closed;
		}

		[[nodiscard]]
		const SvgPoint& GetStartPoint() const
		{
			return _startPoint;
		}

		void Transform(const SvgTransform& transform);

		SvgPathInstance(const SvgPoint& startPoint, std::vector<SvgCubicBezierCurve>&& curves, bool closed);
	private:
		SvgPoint _startPoint;
		std::vector<SvgCubicBezierCurve> _curves;
		SvgBounds _bounds;
		bool _closed;

		static SvgFloat EvaluateBezier(SvgFloat t, SvgFloat p0, SvgFloat p1, SvgFloat p2, SvgFloat p3);
		static void AddBezierPoints(SvgFloat aX, SvgFloat bX, SvgFloat cX, SvgFloat dX, SvgFloat& boundMin, SvgFloat& boundMax);
		static SvgBounds CalculateBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c, const SvgPoint& d);

		void ReCalculateBounds();
	};

} // Elpida

#endif //ELPIDA_SVG_SVGPATHINSTANCE_HPP
