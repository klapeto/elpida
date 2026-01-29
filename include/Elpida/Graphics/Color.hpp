/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 13/9/2024.
//

#ifndef ELPIDA_GRAPHICS_COLOR_HPP
#define ELPIDA_GRAPHICS_COLOR_HPP

namespace Elpida::Graphics
{

	template<typename T>
	class Color
	{
	public:

		Color<T> operator*(const T v) const
		{
			return {
					_r * v,
					_g * v,
					_b * v,
			};
		}

		Color<T>& operator*=(const T& v)
		{
			_r *= v;
			_g *= v;
			_b *= v;
			return *this;
		}

		Color<T>& operator*=(const Color<T>& other)
		{
			_r *= other._r;
			_g *= other._g;
			_b *= other._b;
			return *this;
		}

		Color<T>& operator+=(const Color<T>& other)
		{
			_r += other._r;
			_g += other._g;
			_b += other._b;
			return *this;
		}

		Color<T> operator+(const Color<T>& other) const
		{
			return {
					_r + other._r,
					_g + other._g,
					_b + other._b,
			};
		}

		Color<T> operator*(const Color<T>& other) const
		{
			return {
					_r * other._r,
					_g * other._g,
					_b * other._b,
			};
		}

		[[nodiscard]]
		constexpr T R() const
		{
			return _r;
		}

		[[nodiscard]]
		T& R()
		{
			return _r;
		}

		[[nodiscard]]
		constexpr T G() const
		{
			return _g;
		}

		[[nodiscard]]
		T& G()
		{
			return _g;
		}

		[[nodiscard]]
		constexpr T B() const
		{
			return _b;
		}

		[[nodiscard]]
		T& B()
		{
			return _b;
		}

		[[nodiscard]]
		constexpr T A() const
		{
			return _a;
		}

		[[nodiscard]]
		T& A()
		{
			return _a;
		}

		constexpr Color()
				:_r(0.0), _g(0.0), _b(0.0), _a(1.0)
		{

		}

		constexpr Color(const T r, const T g, const T b)
				:_r(r), _g(g), _b(b), _a(1.0)
		{
		}

		constexpr Color(const T r, const T g, const T b, const T a)
				:_r(r), _g(g), _b(b), _a(a)
		{
		}

		Color(const Color<T>&) = default;
		Color& operator=(const Color<T>&) = default;
		Color(Color<T>&&) noexcept = default;
		Color& operator=(Color<T>&&) noexcept = default;
		~Color() = default;
	private:
		T _r;
		T _g;
		T _b;
		T _a;
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_COLOR_HPP
