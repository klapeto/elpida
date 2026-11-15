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

		// Defined here so that SvgSuperSampler can inline them: called once per sub-sample, an out-of-line call
		// costs the Windows x86_64 ABI's xmm6-xmm9 saves every time (docs/development/parity-checks.md). always_inline
		// keeps the inlining decision the same on every target.
		[[nodiscard, gnu::always_inline]]
		bool IsPointInsideEvenOdd(const SvgPoint& point) const
		{
			bool inside = false;

			auto y = point.GetY();
			auto x = point.GetX();

			for (const auto& edge: _edges)
			{
				auto& a = edge.GetA();
				auto& b = edge.GetB();

				auto aX = a.GetX();
				auto aY = a.GetY();
				auto bX = b.GetX();
				auto bY = b.GetY();

				if (aY > y)
				{
					break;
				}

				if (point == a || point == b)
				{
					// Is exactly at a corner
					inside = true;
					break;
				}

				if (bY > y)
				{
					const auto determinant = (x - aX) * (bY - aY) - (bX - aX) * (y - aY);
					if (determinant == 0)
					{
						// Is directly at line
						inside = true;
						break;
					}

					if (determinant < 0 != (bY < aY))
					{
						inside = !inside;
					}
				}
			}
			return inside;
		}

		[[nodiscard, gnu::always_inline]]
		bool IsPointInsideNonZero(const SvgPoint& point) const
		{
			int inside = 0;
			auto y = point.GetY();
			auto x = point.GetX();
			for (const auto& _edge: _edges)
			{
				auto& a = _edge.GetA();
				auto& b = _edge.GetB();

				auto aX = a.GetX();
				auto aY = a.GetY();
				auto bX = b.GetX();
				auto bY = b.GetY();

				if (aY > y)
				{
					break;
				}

				if (point == a || point == b)
				{
					// Is exactly at a corner
					inside = true;
					break;
				}

				if (bY > y)
				{
					const auto determinant = (x - aX) * (bY - aY) - (bX - aX) * (y - aY);
					if (determinant == 0)
					{
						// Is directly at line
						inside = true;
						break;
					}

					if (determinant < 0 != (bY < aY))
					{
						inside += _edge.GetDirection();
					}
				}
			}
			return inside != 0;
		}

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
