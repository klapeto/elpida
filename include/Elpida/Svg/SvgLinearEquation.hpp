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
		void Translate(const SvgTransform& transform)
		{
			_p1.ApplyTransform(transform);
			_p2.ApplyTransform(transform);
			Recalculate();
		}

		[[nodiscard]]
		double CalculateY(const double x) const
		{
			return _a * x + _b;
		}

		[[nodiscard]]
		double GetDistanceFromPoint(const SvgPoint& point) const
		{
			const auto dx = _p2.GetX() - _p1.GetX();
			const auto dy = _p2.GetY() - _p1.GetY();
			return std::abs(dx * (_p1.GetY() - point.GetY()) - (_p1.GetX() - point.GetX()) * dy) / sqrt(
				dx * dx + dy * dy);
		}

		[[nodiscard]]
		SvgLinearEquation GetPerpendicularEquationFromPoint(const SvgPoint& point) const
		{
			// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
			const auto a = -_a;
			constexpr auto b = 1.0;
			const auto c = -_b;
			const auto x0 = point.GetX();
			const auto y0 = point.GetY();

			const auto x = (b * (b * x0 - a * y0) - a * c) / (a * a + b * b);
			const auto y = (a * (-b * x0 + a * y0) - b * c) / (a * a + b * b);
			return SvgLinearEquation(point, SvgPoint(x, y));
		}

		[[nodiscard]]
		SvgLinearEquation GetANormal() const
		{
			const auto delta = _p2 - _p1;
			const auto a = SvgPoint(-delta.GetY(), delta.GetX());
			const auto b = SvgPoint(delta.GetY(), -delta.GetX());

			const auto dx = _p1.GetX() - a.GetX();
			const auto dy = _p1.GetY() - a.GetY();

			return {
				SvgPoint(a.GetX() + dx, a.GetY() + dy),
				SvgPoint(b.GetX() + dx, b.GetY() + dy)
			};
		}

		[[nodiscard]]
		SvgLinearEquation GetBNormal() const
		{
			const auto delta = _p2 - _p1;
			const auto a = SvgPoint(-delta.GetY(), delta.GetX());
			const auto b = SvgPoint(delta.GetY(), -delta.GetX());

			const auto dx = _p2.GetX() - a.GetX();
			const auto dy = _p2.GetY() - a.GetY();

			return {
				SvgPoint(a.GetX() + dx, a.GetY() + dy),
				SvgPoint(b.GetX() + dx, b.GetY() + dy)
			};
		}

		[[nodiscard]]
		bool IsPointBehindLine(const SvgPoint& point) const
		{
			const auto a = _a;
			const auto c = _b;

			auto sign = a * point.GetX() - point.GetY() + c < 0.0 ? -1 : 1;
			return sign == _direction;
			//return a * point.GetX() - point.GetY() + c < 0.0;
		}

		[[nodiscard]]
		const SvgPoint& GetP1() const
		{
			return _p1;
		}

		[[nodiscard]]
		const SvgPoint& GetP2() const
		{
			return _p2;
		}

		// [[nodiscard]]
		// const SvgPoint& GetDirection() const
		// {
		// 	return _direction;
		// }

		[[nodiscard]]
		double GetDirectionFromPoint(const SvgPoint& point) const
		{
			return point.GetY() - (point.GetX() * _a + _b) > 0.0 ? 1.0 : -1.0;
		}

		SvgLinearEquation(const SvgPoint& a, const SvgPoint& b)
		{
			_p1 = a;
			_p2 = b;
			Recalculate();
		}

		SvgLinearEquation(const double a, const double b)
			: _a(a),
			  _b(b),
		_direction(1.0)
		{
		}

	private:
		SvgPoint _p1;
		SvgPoint _p2;
		double _a;
		double _b;
		int _direction;

		void Recalculate()
		{
			_a = (_p2.GetY() - _p1.GetY()) / (_p2.GetX() - _p1.GetX());
			_b = _p1.GetY() - _a * _p1.GetX();
			_direction = (_p2 - _p1).GetX() > 0.0 ? 1 : -1;
		}
	};
} // Elpida

#endif //ELPIDA_SVG_SVGLINEAREQUATION_HPP
