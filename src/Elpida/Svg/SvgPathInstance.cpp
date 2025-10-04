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

#include "Elpida/Svg/SvgPathInstance.hpp"

namespace Elpida
{
	void SvgPathInstance::Transform(const SvgTransform& transform)
	{
		_startPoint.Transform(transform);

		for (auto& curve : _curves)
		{
			curve.Transform(transform);
		}
		ReCalculateBounds();
	}

	SvgPathInstance::SvgPathInstance(const SvgPoint& startPoint, std::vector<SvgCubicBezierCurve>&& curves,
			const bool closed)
			:_startPoint(startPoint),
			 _curves(std::move(curves)),
			 _closed(closed)
	{
		ReCalculateBounds();
	}

	SvgFloat SvgPathInstance::EvaluateBezier(SvgFloat t, SvgFloat p0, SvgFloat p1, SvgFloat p2, SvgFloat p3)
	{
		const SvgFloat it = SvgFloat(1.0) - t;
		return it * it * it * p0 + SvgFloat(3.0) * it * it * t * p1 + SvgFloat(3.0) * it * t * t * p2 + t * t * t * p3;
	}

	void SvgPathInstance::AddBezierPoints(const SvgFloat aX, const SvgFloat bX, const SvgFloat cX, const SvgFloat dX,
			SvgFloat& boundMin, SvgFloat& boundMax)
	{
		const auto a = SvgFloat(-3.0) * aX + SvgFloat(9.0) * bX - SvgFloat(9.0) * cX + SvgFloat(3.0) * dX;
		const auto b = SvgFloat(6.0) * aX - SvgFloat(12.0) * bX + SvgFloat(6.0) * cX;
		const auto c = SvgFloat(3.0) * bX - SvgFloat(3.0) * aX;
		std::size_t count = 0;

		SvgFloat roots[2];

		constexpr auto epsilon = std::numeric_limits<SvgFloat>::epsilon();

		if (std::abs(a) < epsilon)
		{
			if (std::abs(b) > epsilon)
			{
				const auto t = -c / b;
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
			}
		}
		else
		{
			const auto b2ac = b * b - SvgFloat(4.0) * c * a;
			if (b2ac > epsilon)
			{
				auto t = (-b + std::sqrt(b2ac)) / (SvgFloat(2.0) * a);
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
				t = (-b - std::sqrt(b2ac)) / (SvgFloat(2.0) * a);
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
			}
		}
		for (std::size_t j = 0; j < count; j++)
		{
			const auto v = EvaluateBezier(roots[j], aX, bX, cX, dX);
			boundMin = std::min(boundMin, v);
			boundMax = std::max(boundMax, v);
		}
	}

	SvgBounds SvgPathInstance::CalculateBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c,
			const SvgPoint& d)
	{
		// Start the bounding box by end points
		SvgBounds bounds(a, d);

		// Bezier curve fits inside the convex hull of it's control points.
		// If control points are inside the bounds, we're done.
		if (bounds.Contains(b) && bounds.Contains(c))
		{
			return bounds;
		}

		SvgFloat minX = bounds.GetMinX();
		SvgFloat minY = bounds.GetMinY();
		SvgFloat maxX = bounds.GetMaxX();
		SvgFloat maxY = bounds.GetMaxY();
		// Add bezier curve inflection points in X and Y.
		AddBezierPoints(a.GetX(), b.GetX(), c.GetX(), d.GetX(), minX, maxX);
		AddBezierPoints(a.GetY(), b.GetY(), c.GetY(), d.GetY(), minY, maxY);

		return { minX, minY, maxX, maxY };
	}

	void SvgPathInstance::ReCalculateBounds()
	{
		const auto* currentCurve = &_curves.front();

		_bounds = CalculateBounds(_startPoint, currentCurve->GetStartControl(), currentCurve->GetEndControl(),
				currentCurve->GetEnd());
		for (std::size_t i = 1; i < _curves.size() - 1; ++i)
		{
			auto& curve = _curves[i];
			_bounds.Merge(CalculateBounds(currentCurve->GetEnd(),
					curve.GetStartControl(),
					curve.GetEndControl(),
					curve.GetEnd()));
			currentCurve = &curve;
		}
	}
} // Elpida