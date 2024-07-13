//
// Created by klapeto on 3/1/2024.
//

#include "Elpida/Svg/SvgPathInstance.hpp"

namespace Elpida
{
	const std::vector<SvgCubicBezierCurve>& SvgPathInstance::GetCurves() const
	{
		return _curves;
	}

	const SvgBounds& SvgPathInstance::GetBounds() const
	{
		return _bounds;
	}

	bool SvgPathInstance::IsClosed() const
	{
		return _closed;
	}

	const SvgPoint& SvgPathInstance::GetStartPoint() const
	{
		return _startPoint;
	}

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

	double SvgPathInstance::EvaluateBezier(double t, double p0, double p1, double p2, double p3)
	{
		const double it = 1.0 - t;
		return it * it * it * p0 + 3.0 * it * it * t * p1 + 3.0 * it * t * t * p2 + t * t * t * p3;
	}

	void SvgPathInstance::AddBezierPoints(const double aX, const double bX, const double cX, const double dX,
			double& boundMin, double& boundMax)
	{
		const auto a = -3.0 * aX + 9.0 * bX - 9.0 * cX + 3.0 * dX;
		const auto b = 6.0 * aX - 12.0 * bX + 6.0 * cX;
		const auto c = 3.0 * bX - 3.0 * aX;
		std::size_t count = 0;

		double roots[2];

		constexpr auto epsilon = std::numeric_limits<double>::epsilon();

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
			const auto b2ac = b * b - 4.0 * c * a;
			if (b2ac > epsilon)
			{
				auto t = (-b + std::sqrt(b2ac)) / (2.0 * a);
				if (t > epsilon && t < 1.0 - epsilon)
				{
					roots[count++] = t;
				}
				t = (-b - std::sqrt(b2ac)) / (2.0 * a);
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

		double minX = bounds.GetMinX();
		double minY = bounds.GetMinY();
		double maxX = bounds.GetMaxX();
		double maxY = bounds.GetMaxY();
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