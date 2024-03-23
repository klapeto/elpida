//
// Created by klapeto on 21/3/2024.
//

#ifndef ELPIDA_SVGCIRCLE_HPP
#define ELPIDA_SVGCIRCLE_HPP
#include "SvgPoint.hpp"

namespace Elpida
{
	class SvgCircle
	{
	public:
		[[nodiscard]]
		SvgPoint GetCenter() const
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

		SvgCircle(const SvgPoint &center, double radius)
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

#endif //ELPIDA_SVGCIRCLE_HPP
