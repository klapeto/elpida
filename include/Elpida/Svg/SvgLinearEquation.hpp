/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 30/1/2024.
//

#ifndef ELPIDA_SVG_SVGLINEAREQUATION_HPP
#define ELPIDA_SVG_SVGLINEAREQUATION_HPP

#include "SvgPoint.hpp"
#include "SvgConfig.hpp"

namespace Elpida
{
	class SvgLinearEquation
	{
	public:
		void Transform(const SvgTransform& transform)
		{
			_p1.Transform(transform);
			_p2.Transform(transform);
			Recalculate();
		}

		[[nodiscard]]
		SvgFloat GetA() const
		{
			return _a;
		}

		[[nodiscard]]
		SvgFloat GetB() const
		{
			return _b;
		}

		[[nodiscard]]
		SvgFloat CalculateY(const SvgFloat x) const
		{
			return _a * x + _b;
		}

		[[nodiscard]]
		SvgFloat GetDistanceFromPoint(const SvgPoint& point) const
		{
			const auto dx = _p2.GetX() - _p1.GetX();
			const auto dy = _p2.GetY() - _p1.GetY();
			return std::abs(dx * (_p1.GetY() - point.GetY()) - (_p1.GetX() - point.GetX()) * dy)
					/ std::sqrt(dx * dx + dy * dy);
		}

		[[nodiscard]]
		SvgLinearEquation GetPerpendicularEquationFromPoint(const SvgPoint& point) const
		{
			if (std::abs(_direction.GetX()) < SvgPoint::Tolerance)
			{
				// vertical line
				return SvgLinearEquation(point, SvgPoint(_p1.GetX(), point.GetY()));
			}

			if (std::abs(_direction.GetY()) < SvgPoint::Tolerance)
			{
				// horizontal line
				return SvgLinearEquation(point, SvgPoint(point.GetX(), _p1.GetY()));
			}

			// See https://en.wikipedia.org/wiki/Distance_from_a_point_to_a_line
			const auto a = -_a;
			constexpr SvgFloat b = 1.0;
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
		SvgFloat CalculateAngle() const
		{
			return std::atan2(_direction.GetY(), _direction.GetX());
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
		bool IsPointBehindLine(const SvgPoint& point, const SvgPoint& direction) const
		{
			if (std::abs(_direction.GetX()) < SvgPoint::Tolerance)
			{
				// vertical line
				const auto sign = GetSign(point.GetX() - _p1.GetX());
				if (sign == 0) return true;
				return sign != GetSign(direction.GetX());
			}

			if (std::abs(_direction.GetY()) < SvgPoint::Tolerance)
			{
				// horzizontal line
				const auto sign = GetSign(point.GetY() - _p1.GetY());
				if (sign == 0) return true;
				return sign != GetSign(direction.GetY());
			}

			const auto sign = GetSign(_a * point.GetX() - point.GetY() + _b);
			//const auto sign = GetSign(CalculateY(point.GetX()));
			if (sign == 0) return true;
			return sign == GetSign(direction.GetY());
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

		[[nodiscard]]
		SvgPoint GetDirectionFromPoint(const SvgPoint& point) const
		{
			return point - _p2;
		}

		[[nodiscard]]
		const SvgPoint& GetDirection() const
		{
			return _direction;
		}

		SvgLinearEquation()
				:_a(0.0), _b(0.0)
		{

		}

		SvgLinearEquation(const SvgPoint& a, const SvgPoint& b)
		{
			_p1 = a;
			_p2 = b;
			Recalculate();
		}

		SvgLinearEquation(const SvgFloat a, const SvgFloat b)
				:_a(a),
				 _b(b)
		{
		}

	private:
		SvgPoint _p1;
		SvgPoint _p2;
		SvgFloat _a;
		SvgFloat _b;
		SvgPoint _direction;

		void Recalculate()
		{
			_direction = _p2 - _p1;
			_a = std::abs(_direction.GetX()) > SvgPoint::Tolerance ? (_direction.GetY()) / (_direction.GetX()) : SvgFloat(0.0);
			_b = _p1.GetY() - _a * _p1.GetX();
		}

		static int GetSign(const SvgFloat v)
		{
			if (std::abs(v) < SvgPoint::Tolerance) return 0;
			if (v == 0.0) return 0;
			return v < 0.0 ? -1 : 1;
		}
	};
} // Elpida

#endif //ELPIDA_SVG_SVGLINEAREQUATION_HPP
