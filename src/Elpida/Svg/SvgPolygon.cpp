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
// Created by klapeto on 6/3/2024.
//

#include "Elpida/Svg/SvgPolygon.hpp"

#include <algorithm>

namespace Elpida
{
	void SvgPolygon::CalculateBounds()
	{
		if (_edges.empty())
		{
			_bounds = {};
			return;
		}
		_bounds = SvgBounds::CreateMinimum();
		for (auto edge: _edges)
		{
			_bounds.Merge(SvgBounds(edge.GetA(), edge.GetB()));
		}

		std::sort(_edges.begin(), _edges.end(), [](SvgEdge& a, SvgEdge& b)
		{
			return a.GetA().GetY() < b.GetA().GetY();
		});
	}

	void SvgPolygon::Transform(const SvgTransform& transform)
	{
		for (auto& edge: _edges)
		{
			edge.Transform(transform);
		}

		CalculateBounds();

	}
} // Elpida
