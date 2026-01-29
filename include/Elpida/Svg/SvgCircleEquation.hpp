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
// Created by klapeto on 24/3/2024.
//

#ifndef ELPIDA_SVGCIRCLEEQUATION_HPP
#define ELPIDA_SVGCIRCLEEQUATION_HPP

#include "Elpida/Svg/SvgPoint.hpp"
#include "Elpida/Svg/SvgConfig.hpp"

namespace Elpida
{
	class SvgCircleEquation
	{
	public:
		[[nodiscard]]
		const SvgPoint& GetCenter() const
		{
			return _center;
		}

		[[nodiscard]]
		SvgFloat GetRadius() const
		{
			return _radius;
		}

		void ScaleBy(const SvgFloat value)
		{
			_rightPoint = SvgPoint(_rightPoint.GetX() + value, _rightPoint.GetY());
			Recalculate();
		}

		[[nodiscard]]
		bool IsPointInside(const SvgPoint& point) const
		{
			return _center.GetDistance(point) <= _radius;
		}

		SvgCircleEquation(const SvgPoint &center, SvgFloat radius)
			: _center(center),
			  _radius(radius)
		{
			_rightPoint = SvgPoint(_center.GetX() + _radius, _center.GetY());
		}

	private:
		SvgPoint _center;
		SvgPoint _rightPoint;
		SvgFloat _radius;

		void Recalculate()
		{
			_radius = _center.GetDistance(_rightPoint);
		}
	};
} // Elpida

#endif //ELPIDA_SVGCIRCLEEQUATION_HPP
