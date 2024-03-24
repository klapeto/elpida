//
// Created by klapeto on 24/3/2024.
//

#ifndef ELPIDA_SVGCIRCLEEQUATION_HPP
#define ELPIDA_SVGCIRCLEEQUATION_HPP

#include "Elpida/Svg/SvgPoint.hpp"

namespace Elpida
{
	class SvgCircleEquation
	{
	public:
		[[nodiscard]]
		const SvgPoint& GetCenter() const
		{
			return _center;
		}

		[[nodiscard]]
		double GetRadius() const
		{
			return _radius;
		}

		void ScaleBy(const double value)
		{
			_rightPoint = SvgPoint(_rightPoint.GetX() + value, _rightPoint.GetY());
			Recalculate();
		}

		[[nodiscard]]
		bool IsPointInside(const SvgPoint& point) const
		{
			return _center.GetDistance(point) <= _radius;
		}

		SvgCircleEquation(const SvgPoint &center, double radius)
			: _center(center),
			  _radius(radius)
		{
			_rightPoint = SvgPoint(_center.GetX() + _radius, _center.GetY());
		}

	private:
		SvgPoint _center;
		SvgPoint _rightPoint;
		double _radius;

		void Recalculate()
		{
			_radius = _center.GetDistance(_rightPoint);
		}
	};
} // Elpida

#endif //ELPIDA_SVGCIRCLEEQUATION_HPP
