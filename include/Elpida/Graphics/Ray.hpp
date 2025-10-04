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
// Created by klapeto on 14/9/2024.
//

#ifndef ELPIDA_GRAPHICS_RAY_HPP
#define ELPIDA_GRAPHICS_RAY_HPP

#include "Elpida/Graphics/Vector3.hpp"

namespace Elpida::Graphics
{
	template<typename T>
	class Ray
	{
	public:
		const Vector3<T>& GetOrigin() const
		{
			return _origin;
		}

		const Vector3<T>& GetDirection() const
		{
			return _direction;
		}


		Ray() = default;

		Ray(const Vector3<T>& origin, const Vector3<T>& direction)
				:_origin(origin), _direction(direction)
		{
		}

		Ray(const Ray<T>&) = default;
		Ray& operator=(const Ray<T>&) = default;
		Ray(Ray<T>&&) noexcept = default;
		Ray& operator=(Ray<T>&&) noexcept = default;
		~Ray() = default;
	private:
		Vector3<T> _origin;
		Vector3<T> _direction;
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_RAY_HPP
