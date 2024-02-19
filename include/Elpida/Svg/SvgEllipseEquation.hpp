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
		bool IsPointInside(const SvgPoint& point) const
		{
			const auto dx = point.GetX() - _center.GetX();
			const auto dy = point.GetY() - _center.GetY();

			const auto value = ((dx * dx) / (_radius.GetX() * _radius.GetX()))
					+ ((dy * dy) / (_radius.GetY() * _radius.GetY()));
			return value <= 1.0;
		}

		SvgEllipseEquation(const SvgPoint& center, const double rX, const double rY)
			: _center(center), _radius(rX, rY)
		{
		}

	private:
		SvgPoint _center;
		SvgPoint _radius;
	};
} // Elpida

#endif //SVGELLIPSE_HPP
