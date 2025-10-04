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
// Created by klapeto on 28/12/2023.
//

#ifndef ELPIDA_SVG_SVGPOINT_HPP
#define ELPIDA_SVG_SVGPOINT_HPP

#include <algorithm>

#include "SvgTransform.hpp"
#include "SvgConfig.hpp"

namespace Elpida
{
	class SvgPoint
	{
	public:

		constexpr static SvgFloat Tolerance = 0.000001;

		[[nodiscard]]
		SvgFloat GetX() const
		{
			return _x;
		}

		[[nodiscard]]
		SvgFloat GetY() const
		{
			return _y;
		}

		[[nodiscard]]
		SvgFloat& GetRefX()
		{
			return _x;
		}

		[[nodiscard]]
		SvgFloat& GetRefY()
		{
			return _y;
		}

		void operator+=(const SvgPoint& other)
		{
			_x += other._x;
			_y += other._y;
		}

		void operator-=(const SvgPoint& other)
		{
			_x -= other._x;
			_y -= other._y;
		}

		SvgPoint operator*(const SvgFloat value) const
		{
			return { _x * value, _y * value };
		}

		SvgPoint operator+(const SvgPoint& other) const
		{
			return { _x + other._x, _y + other._y };
		}

		SvgPoint operator-(const SvgPoint& other) const
		{
			return { _x - other._x, _y - other._y };
		}

		SvgPoint& operator*=(const SvgFloat value)
		{
			_x *= value;
			_y *= value;
			return *this;
		}

		SvgPoint& operator/=(const SvgFloat value)
		{
			_x /= value;
			_y /= value;
			return *this;
		}

		SvgPoint operator-() const
		{
			return { -_x, -_y };
		}

		SvgPoint operator/(const SvgFloat x) const
		{
			return { _x / x, _y / x };
		}

		void Transform(const SvgTransform& transform)
		{
			transform.ApplyToPoint(_x, _y);
		}

		void ToMin(const SvgPoint& point)
		{
			_x = std::min(_x, point._x);
			_y = std::min(_y, point._y);
		}

		void ToMax(const SvgPoint& point)
		{
			_x = std::max(_x, point._x);
			_y = std::max(_y, point._y);
		}

		[[nodiscard]]
		SvgFloat Product() const
		{
			return _x * _x + _y * _y;
		}

		[[nodiscard]]
		SvgFloat Length() const
		{
			return std::sqrt(Product());
		}

		[[nodiscard]]
		SvgPoint GetInverse() const
		{
			return { _y, -_x };
		}

		SvgFloat Normalize()
		{
			const SvgFloat length = Length();
			if (length > Tolerance)
			{
				const SvgFloat id = SvgFloat(1.0) / length;
				_x *= id;
				_y *= id;
			}
			return length;
		}

		bool operator==(const SvgPoint& other) const
		{
			constexpr auto distanceTolerance = Tolerance * Tolerance;
			const SvgFloat dx = _x - other._x;
			const SvgFloat dy = _y - other._y;
			return dx * dx + dy * dy < distanceTolerance;
		}

		[[nodiscard]]
		SvgFloat GetDistance(const SvgPoint& other) const
		{
			const SvgFloat dx = _x - other._x;
			const SvgFloat dy = _y - other._y;
			return std::sqrt(dx * dx + dy * dy);
		}

		[[nodiscard]]
		SvgFloat GetSlope(const SvgPoint& to) const
		{
			const SvgFloat dx = to._x - _x;
			const SvgFloat dy = to._y - _y;
			return dy / dx;
		}

		SvgPoint()
				:_x(0.0), _y(0.0)
		{
		}

		SvgPoint(const SvgFloat x, const SvgFloat y)
				:_x(x),
				 _y(y)
		{
		}

		SvgPoint(const SvgPoint&) = default;
		SvgPoint(SvgPoint&&) noexcept = default;
		SvgPoint& operator=(const SvgPoint&) = default;
		SvgPoint& operator=(SvgPoint&&) noexcept = default;
		~SvgPoint() = default;
	private:
		SvgFloat _x;
		SvgFloat _y;
	};
} // Elpida

#endif //ELPIDA_SVG_SVGPOINT_HPP
