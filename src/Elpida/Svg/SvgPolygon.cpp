//
// Created by klapeto on 6/3/2024.
//

#include "Elpida/Svg/SvgPolygon.hpp"

namespace Elpida
{
	bool SvgPolygon::IsPointInsideEvenOdd(const SvgPoint& point) const
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

	bool SvgPolygon::IsPointInsideNonZero(const SvgPoint& point) const
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

	void SvgPolygon::CalculateBounds()
	{
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
