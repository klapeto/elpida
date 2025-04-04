//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 12/9/2024.
//

#ifndef ELPIDA_GRAPHICS_VECTOR3_HPP
#define ELPIDA_GRAPHICS_VECTOR3_HPP

#include <type_traits>
#include <cmath>

#include "Elpida/Math/Matrix.hpp"

namespace Elpida::Graphics
{

	template<typename T>
	class Vector3
	{
	public:

		constexpr T operator[](const std::size_t i) const
		{
			return ((T*)&_x)[i];
		}

		constexpr T X() const
		{
			return _x;
		}

		constexpr T Y() const
		{
			return _y;
		}

		constexpr T Z() const
		{
			return _z;
		}

		template<std::size_t R, std::size_t C>
		Vector3<T>& Transform(const Math::Matrix<T, R, C>& matrix)
		{
			auto m = Math::Matrix<T, R, 1>::WithOnes(_x, _y, _z);
			auto multiple = matrix * m;
			_x = multiple[0];
			_y = multiple[1];
			_z = multiple[2];
			return *this;
		}

		constexpr T LengthSquared() const
		{
			return (_x * _x) + (_y * _y) + (_z * _z);
		}

		constexpr T Length() const
		{
			return std::sqrt(LengthSquared());
		}

		Vector3<T>& operator+=(const Vector3<T>& other)
		{
			_x += other._x;
			_y += other._y;
			_z += other._z;
			return *this;
		}

		Vector3<T>& operator-=(const Vector3<T>& other)
		{
			_x -= other._x;
			_y -= other._y;
			_z -= other._z;
			return *this;
		}

		Vector3<T>& operator*=(T scale)
		{
			_x *= scale;
			_y *= scale;
			_z *= scale;
			return *this;
		}

		Vector3<T> operator+(const Vector3<T>& other) const
		{
			return {
					_x + other._x,
					_y + other._y,
					_z + other._z,
			};
		}

		Vector3<T> operator-(const Vector3<T>& other) const
		{
			return {
					_x - other._x,
					_y - other._y,
					_z - other._z,
			};
		}

		T DotProduct(const Vector3<T>& other) const
		{
			return (_x * other._x) + (_y * other._y) + (_z * other._z);
		}

		Vector3<T> operator*(const Vector3<T>& other) const
		{
			return {
					_x * other._x,
					_y * other._y,
					_z * other._z,
			};
		}

		Vector3<T> Cross(const Vector3<T>& other) const
		{
			return {
					(_y * other._z) - (_z * other._y),
					(_z * other._x) - (_x * other._z),
					(_x * other._y) - (_y * other._x)
			};
		}

		Vector3<T> operator*(T scale) const
		{
			return {
					_x * scale,
					_y * scale,
					_z * scale,
			};
		}

		Vector3<T>& Normalize()
		{
			auto length = Length();

			_x /= length;
			_y /= length;
			_z /= length;
			return *this;
		}

		constexpr Vector3<T> Normalized() const
		{
			auto length = Length();

			return {
					_x / length,
					_y / length,
					_z / length
			};
		}

		constexpr Vector3()
				:_x(0.0), _y(0.0), _z(0.0)
		{
		}

		constexpr Vector3(T x, T y, T z)
				:_x(x), _y(y), _z(z)
		{
		}

		Vector3(const Vector3<T>&) = default;
		Vector3& operator=(const Vector3<T>&) = default;
		Vector3(Vector3<T>&&) noexcept = default;
		Vector3& operator=(Vector3<T>&&) noexcept = default;
		~Vector3() = default;
	protected:
		static_assert(std::is_arithmetic<T>::value, "Vertex requires arithmetic template parameter");
		T _x;
		T _y;
		T _z;
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_VECTOR3_HPP
