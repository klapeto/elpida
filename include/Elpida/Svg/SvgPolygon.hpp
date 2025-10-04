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

#ifndef ELPIDA_SVG_SVGPOLYGON_HPP
#define ELPIDA_SVG_SVGPOLYGON_HPP

#include <vector>

#include "SvgBounds.hpp"
#include "SvgEdge.hpp"

namespace Elpida
{

	class SvgTransform;

	class SvgPolygon
	{
	public:
		[[nodiscard]]
		const std::vector<SvgEdge>& GetEdges() const
		{
			return _edges;
		}

		[[nodiscard]]
		const SvgBounds& GetBounds() const
		{
			return _bounds;
		}

		[[nodiscard]]
		bool IsPointInsideEvenOdd(const SvgPoint& point) const;

		[[nodiscard]]
		bool IsPointInsideNonZero(const SvgPoint& point) const;

		void Transform(const SvgTransform& transform);

		SvgPolygon() = default;
		explicit SvgPolygon(std::vector<SvgEdge>&& edges)
				:_edges(std::move(edges))
		{
			CalculateBounds();
		}

	private:
		std::vector<SvgEdge> _edges;
		SvgBounds _bounds;

		void CalculateBounds();
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPOLYGON_HPP
