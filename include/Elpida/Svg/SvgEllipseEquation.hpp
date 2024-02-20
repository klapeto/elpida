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
			_focusA.Transform(transform);
			_focusB.Transform(transform);
			_topPoint.Transform(transform);
			_constantDistance = _focusA.GetDistance(_topPoint) + _focusB.GetDistance(_topPoint);
		}

		[[nodiscard]]
		bool IsPointInside(const SvgPoint& point) const
		{
			auto distance = _focusA.GetDistance(point) + _focusB.GetDistance(point);

			return distance <= _constantDistance;
		}

		SvgEllipseEquation(const SvgPoint& center, const double rX, const double rY)
		{
			auto focus = sqrt(rX * rX - rY * rY);
			_focusA = SvgPoint(center.GetX() + focus, center.GetY());
			_focusB = SvgPoint(center.GetX() - focus, center.GetY());
			_constantDistance = rX * 2;
			_topPoint = SvgPoint(center.GetX(), center.GetY() - rY);
		}

	private:
		SvgPoint _topPoint;
		SvgPoint _focusA;
		SvgPoint _focusB;
		double _constantDistance;
	};
} // Elpida

#endif //SVGELLIPSE_HPP
