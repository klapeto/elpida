//
// Created by klapeto on 30/1/2024.
//

#ifndef ELPIDA_SVG_SVGLINEAREQUATION_HPP
#define ELPIDA_SVG_SVGLINEAREQUATION_HPP
#include "SvgPoint.hpp"

namespace Elpida
{
	class SvgLinearEquation
	{
	public:
		[[nodiscard]]
		double CalculateY(const double x) const
		{
			return _a * x + _b;
		}

		[[nodiscard]]
		double GetDistanceFromPoint(const SvgPoint& point) const
		{
			auto dx = _p2.GetX() - _p1.GetX();
			auto dy = _p2.GetY() - _p1.GetY();
			return std::abs(dx * (_p1.GetY() - point.GetY()) - (_p1.GetX() - point.GetX())  * dy) / sqrt(dx * dx + dy*dy);
		}

		SvgLinearEquation(const SvgPoint& a, const SvgPoint& b)
		{
			_p1 = a;
			_p2 = b;
			_a = (b.GetY() - a.GetY()) / (b.GetX() - a.GetX());
			_b = a.GetY() - _a * a.GetX();
		}

		SvgLinearEquation(const double a, const double b)
			: _a(a),
			  _b(b)
		{
		}

	private:
		SvgPoint _p1;
		SvgPoint _p2;
		double _a;
		double _b;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGLINEAREQUATION_HPP
