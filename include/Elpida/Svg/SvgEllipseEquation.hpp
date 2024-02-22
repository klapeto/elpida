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

		[[nodiscard]]
		const SvgPoint& GetCenter() const
		{
			return _center;
		}

		[[nodiscard]]
		const SvgPoint& GetTopPoint() const
		{
			return _topPoint;
		}

		[[nodiscard]]
		const SvgPoint& GetRightPoint() const
		{
			return _rightPoint;
		}

		[[nodiscard]]
		const SvgPoint& GetFocusA() const
		{
			return _focusPointA;
		}

		[[nodiscard]]
		const SvgPoint& GetFocusB() const
		{
			return _focusPointB;
		}

		[[nodiscard]]
		double GetConstantDistance() const
		{
			return _constantDistance;
		}

		void Transform(const SvgTransform& transform)
		{
			_focusPointA.Transform(transform);
			_focusPointB.Transform(transform);
			_topPoint.Transform(transform);
			_rightPoint.Transform(transform);
			_center.Transform(transform);
			_constantDistance = _focusPointA.GetDistance(_topPoint) + _focusPointB.GetDistance(_topPoint);
		}

		[[nodiscard]]
		bool IsPointInside(const SvgPoint& point) const
		{
			const auto distance = _focusPointA.GetDistance(point) + _focusPointB.GetDistance(point);

			return distance <= _constantDistance;
		}

		SvgEllipseEquation(const double cX, const double cY, const double rX, const double rY)
		{
			// An ellipse can be defined by the focus points instead of the general equation
			// we opt for this because this makes the IsPointInside() calculation indipendent from
			// the rotation of the ellipse. Using the math we calculate the focus points.
			// On an ellipse, each point on the ellipse outline, the sum of the distances from the
			// focus points is always equal to 2 * long axis length. Depending on which axis is longer
			// we calculate the focus points as well as a point on the ellipsis (_topPoint) to use
			// to recalulcate the constant distance when transformations happen.

			_center = SvgPoint(cX, cY);
			if (rX > rY)
			{
				const auto focus = sqrt(rX * rX - rY * rY);
				_focusPointA = SvgPoint(cX + focus, cY);
				_focusPointB = SvgPoint(cX - focus, cY);
				_constantDistance = rX * 2;
				_topPoint = SvgPoint(cX, cY - rY);
				_rightPoint = SvgPoint(cX + rX, cY);
			}
			else
			{
				const auto focus = sqrt(rY * rY - rX * rX);
				_focusPointA = SvgPoint(cX, cY + focus);
				_focusPointB = SvgPoint(cX, cY - focus);
				_constantDistance = rY * 2;
				_topPoint = SvgPoint(cX + rX, cY);
				_rightPoint = SvgPoint(cX, cY - rY);
			}

		}

	private:
		SvgPoint _center;
		SvgPoint _rightPoint;
		SvgPoint _topPoint;
		SvgPoint _focusPointA;
		SvgPoint _focusPointB;
		double _constantDistance;
	};
} // Elpida

#endif //SVGELLIPSE_HPP
