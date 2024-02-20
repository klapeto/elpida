//
// Created by klapeto on 18/2/2024.
//

#ifndef ELPIDA_SVG_SVGELLIPSE_HPP
#define ELPIDA_SVG_SVGELLIPSE_HPP

#include "SvgPoint.hpp"

namespace Elpida
{
	class SvgEllipseEquation
	{
	public:

		void Transform(const SvgTransform& transform)
		{
			_focusPointA.Transform(transform);
			_focusPointB.Transform(transform);
			_topPoint.Transform(transform);
			_constantDistance = _focusPointA.GetDistance(_topPoint) + _focusPointB.GetDistance(_topPoint);
		}

		[[nodiscard]]
		bool IsPointInside(const SvgPoint& point) const
		{
			const auto distance = _focusPointA.GetDistance(point) + _focusPointB.GetDistance(point);

			return distance <= _constantDistance;
		}

		SvgEllipseEquation(const SvgPoint& center, const double rX, const double rY)
		{
			// An ellipse can be defined by the focus points instead of the general equation
			// we opt for this because this makes the IsPointInside() calculation indipendent from
			// the rotation of the ellipse. Using the math we calculate the focus points.
			// On an ellipse, each point on the ellipse outline, the sum of the distances from the
			// focus points is always equal to 2 * long axis length. Depending on which axis is longer
			// we calculate the focus points as well as a point on the ellipsis (_topPoint) to use
			// to recalulcate the constant distance when transformations happen.

			if (rX > rY)
			{
				const auto focus = sqrt(rX * rX - rY * rY);
				_focusPointA = SvgPoint(center.GetX() + focus, center.GetY());
				_focusPointB = SvgPoint(center.GetX() - focus, center.GetY());
				_constantDistance = rX * 2;
				_topPoint = SvgPoint(center.GetX(), center.GetY() - rY);
			}
			else
			{
				const auto focus = sqrt(rY * rY - rX * rX);
				_focusPointA = SvgPoint(center.GetX(), center.GetY() + focus);
				_focusPointB = SvgPoint(center.GetX(), center.GetY() - focus);
				_constantDistance = rY * 2;
				_topPoint = SvgPoint(center.GetX() + rX, center.GetY());
			}

		}

	private:
		SvgPoint _topPoint;
		SvgPoint _focusPointA;
		SvgPoint _focusPointB;
		double _constantDistance;
	};
} // Elpida

#endif //SVGELLIPSE_HPP
