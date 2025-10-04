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
// Created by klapeto on 12/9/2024.
//

#ifndef ELPIDA_GRAPHICS_TRIANGLE_HPP
#define ELPIDA_GRAPHICS_TRIANGLE_HPP

#include <cmath>
#include <limits>

#include "Elpida/Graphics/Vector3.hpp"
#include "Elpida/Graphics/Color.hpp"
#include "Elpida/Graphics/Shader.hpp"

namespace Elpida::Graphics
{

	template<typename T>
	class Triangle
	{
	public:

		const Vector3<T>& GetNormal() const
		{
			return _normal;
		}

		T IntersectsRay(const Vector3<T>& origin, const Vector3<T>& direction, T near = 0.001,
				T far = std::numeric_limits<T>::max()) const
		{
			auto u = (_axis + 1) % 3;
			auto v = (_axis + 2) % 3;
			auto d = direction[_axis] + _nu * direction[u] + _nv * direction[v];
			auto t = (_nd - origin[_axis] - _nu * origin[u] - _nv * origin[v]) / d;
			if (t < near || t > far)
			{
				return -1;
			}
			auto Pu = origin[u] + t * direction[u] - _eu;
			auto Pv = origin[v] + t * direction[v] - _ev;
			auto a2 = Pv * _nu1 + Pu * _nv1;
			if (a2 < 0)
			{
				return -1;
			}
			auto a3 = Pu * _nu2 + Pv * _nv2;
			if (a3 < 0)
			{
				return -1;
			}

			if ((a2 + a3) > 1)
			{
				return -1;
			}
			return t;
		}

		Shader<T> GetShader() const
		{
			return _shader;
		}

		Triangle()
				:_axis(0), _nu(0.0), _nv(0.0), _nd(0.0), _eu(0.0), _nu1(0.0), _nv1(0.0), _nu2(0.0), _nv2(0.0)
		{

		}

		Triangle(const Vector3<T>& a, const Vector3<T>& b, const Vector3<T>& c, Shader<T> shader = [](auto p){ return ShaderColor<T>(); })
				:_a(a), _b(b), _c(c), _shader(shader), _axis(0), _nu(0.0), _nv(0.0), _nd(0.0), _eu(0.0), _nu1(0.0),
				 _nv1(0.0), _nu2(0.0), _nv2(0.0)
		{
			Calculate();
		}

		Triangle(const Triangle<T>&) = default;
		Triangle& operator=(const Triangle<T>&) = default;
		Triangle(Triangle<T>&&) noexcept = default;
		Triangle& operator=(Triangle<T>&&) noexcept = default;
		~Triangle() = default;
	private:
		Vector3<T> _a;
		Vector3<T> _b;
		Vector3<T> _c;
		Vector3<T> _normal;
		Shader<T> _shader;

		std::size_t _axis;
		T _nu;
		T _nv;
		T _nd;
		T _eu;
		T _ev;
		T _nu1;
		T _nv1;
		T _nu2;
		T _nv2;

		void Calculate()
		{
			auto edge1 = _c - _a;
			auto edge2 = _b - _a;
			_normal = edge1.Cross(edge2).Normalize();
			auto normal = edge1.Cross(edge2);

			if (std::abs(normal.X()) > std::abs(normal.Y()))
			{
				if (std::abs(normal.X()) > std::abs(normal.Z()))
				{
					_axis = 0;
				}
				else
				{
					_axis = 2;
				}
			}
			else
			{
				if (std::abs(normal.Y()) > std::abs(normal.Z()))
				{
					_axis = 1;
				}
				else
				{
					_axis = 2;
				}
			}

			auto u = (_axis + 1) % 3;
			auto v = (_axis + 2) % 3;

			auto u1 = edge1[u];
			auto v1 = edge1[v];

			auto u2 = edge2[u];
			auto v2 = edge2[v];

			_normal = normal.Normalized();

			_nu = normal[u] / normal[_axis];
			_nv = normal[v] / normal[_axis];
			_nd = normal.DotProduct(_a) / normal[_axis];
			auto det = u1 * v2 - v1 * u2;
			_eu = _a[u];
			_ev = _a[v];
			_nu1 = u1 / det;
			_nv1 = -v1 / det;
			_nu2 = v2 / det;
			_nv2 = -u2 / det;
		}
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_TRIANGLE_HPP
