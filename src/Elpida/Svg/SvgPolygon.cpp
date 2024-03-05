//
// Created by klapeto on 6/3/2024.
//

#include "Elpida/Svg/SvgPolygon.hpp"

namespace Elpida
{
	bool SvgPolygon::IsPointInsideEvenOdd(const SvgPoint& point) const
	{
		bool inside = false;
		for (const auto& edge : _edges)
		{
			auto& a = edge.GetA();
			auto& b = edge.GetB();

			if (point == a || point == b)
			{
				// Is exactly at a corner
				inside = true;
				break;
			}

			if (a.GetY() > point.GetY() != b.GetY() > point.GetY())
			{
				const auto determinant = (point.GetX() - a.GetX()) * (b.GetY() - a.GetY()) - (b.GetX() - a.GetX()) * (
					point.GetY() - a.GetY());
				if (determinant == 0)
				{
					// Is directly at line
					inside = true;
					break;
				}

				if (determinant < 0 != (b.GetY() < a.GetY()))
				{
					inside = !inside;
				}
			}
		}
		return inside;
	}

	bool SvgPolygon::IsPointInsideNonZero(const SvgPoint& point) const
	{
		int inside = 0;
		for (const auto& _edge : _edges)
		{
			auto& a = _edge.GetA();
			auto& b = _edge.GetB();

			if (point == a || point == b)
			{
				// Is exactly at a corner
				inside = true;
				break;
			}

			if (a.GetY() > point.GetY() != b.GetY() > point.GetY())
			{
				const auto determinant = (point.GetX() - a.GetX()) * (b.GetY() - a.GetY()) - (b.GetX() - a.GetX()) * (
					point.GetY() - a.GetY());
				if (determinant == 0)
				{
					// Is directly at line
					inside = true;
					break;
				}

				if (determinant < 0 != (b.GetY() < a.GetY()))
				{
					inside += _edge.GetDirection();
				}
			}
		}
		return inside != 0;
	}
} // Elpida
