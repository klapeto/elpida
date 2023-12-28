//
// Created by klapeto on 7/12/2023.
//

#include "Elpida/Svg/SvgBounds.hpp"

#include "Elpida/Svg/SvgPoint.hpp"

namespace Elpida
{
	double Min(const double a, const double b)
	{
		return a < b ? a : b;
	}

	double Max(const double a, const double b)
	{
		return a > b ? a : b;
	}

	static double EvaluateBezier(const double t, const double p0, const double p1, const double p2, const double p3)
	{
		const double it = 1.0 - t;
		return it * it * it * p0 + 3.0 * it * it * t * p1 + 3.0 * it * t * t * p2 + t * t * t * p3;
	}

	static void AddBeizerPoints(const double aX, const double bX, const double cX, const double dX, double& boundMin,
	                            double& boundMax)
	{
		const auto a = -3.0 * aX + 9.0 * bX - 9.0 * cX + 3.0 * dX;
		const auto b = 6.0 * aX - 12.0 * bX + 6.0 * cX;
		const auto c = 3.0 * bX - 3.0 * aX;
		std::size_t count = 0;

		double roots[2];

		constexpr auto epsilon = std::numeric_limits<double>::epsilon();

		if (fabs(a) < epsilon)
		{
			if (fabs(b) > epsilon)
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
			const auto b2ac = b * b - 4.0 * c * a;
			if (b2ac > epsilon)
			{
				auto t = (-b + sqrt(b2ac)) / (2.0 * a);
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
				t = (-b - sqrt(b2ac)) / (2.0 * a);
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
			}
		}
		for (std::size_t j = 0; j < count; j++)
		{
			const auto v = EvaluateBezier(roots[j], aX, bX, cX, dX);
			boundMin = Min(boundMin, v);
			boundMax = Max(boundMax, v);
		}
	}

	void SvgBounds::Merge(const SvgBounds& other)
	{
		_minX = Min(_minX, other._minX);
		_minY = Min(_minY, other._minY);
		_maxX = Max(_maxX, other._maxX);
		_maxY = Max(_maxY, other._maxY);
	}

	SvgBounds::SvgBounds(const SvgPoint& a, const SvgPoint& b, const SvgPoint& c, const SvgPoint& d)
	{
		// Start the bounding box by end points
		_minX = Min(a.GetX(), d.GetX());
		_minY = Min(a.GetY(), d.GetY());
		_maxX = Max(a.GetX(), d.GetX());
		_maxY = Max(a.GetY(), d.GetY());

		// Bezier curve fits inside the convex hull of it's control points.
		// If control points are inside the bounds, we're done.
		if (Contains(b) && Contains(c))
		{
			return;
		}

		// Add bezier curve inflection points in X and Y.
		AddBeizerPoints(a.GetX(), b.GetX(), c.GetX(), d.GetX(), _minX, _maxX);
		AddBeizerPoints(a.GetY(), b.GetY(), c.GetY(), d.GetY(), _minY, _maxY);
	}

	bool SvgBounds::Contains(const SvgPoint& point) const
	{
		return point.GetX() >= _minX && point.GetX() <= _maxX && point.GetY() >= _minY && point.GetY() <= _maxY;
	}
} // Elpida
