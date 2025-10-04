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

#ifndef ELPIDA_GRAPHICS_CAMERA_HPP
#define ELPIDA_GRAPHICS_CAMERA_HPP

#include "Elpida/Graphics/Vector3.hpp"
#include "Elpida/Graphics/Ray.hpp"
#include "Elpida/Graphics/FrameBuffer.hpp"
#include "Elpida/Graphics/Scene.hpp"
#include "Elpida/Math/Matrix.hpp"

namespace Elpida::Graphics
{
	template<typename T>
	class Camera
	{
	public:

		void Render(const Scene<T>& scene, FrameBuffer<T>& frameBuffer) const
		{
			auto height = frameBuffer.GetHeight();
			auto width = frameBuffer.GetWidth();

			Ray<T> a, b;
			for (std::size_t y = 0; y < height; ++y)
			{
				GenerateRayPair(y / T(height), a, b);
				for (std::size_t x = 0; x < width; ++x)
				{
					auto xp = x / T(width);
					auto origin = (a.GetOrigin() * xp) + (b.GetOrigin() * (1.0 - xp));
					auto direction = ((a.GetDirection() * xp) + (b.GetDirection() * (1.0 - xp))).Normalize();
					auto l = scene.Intersect(origin, direction);
					frameBuffer.Get(x, y) = l;
				}
			}
		}

		Camera() = default;
		Camera(const Vector3<T>& origin, const Vector3<T>& lookAt, const Vector3<T>& up)
				:_origin(origin)
		{
			auto zAxis = (lookAt - origin).Normalize();
			auto xAxis = up.Cross(zAxis).Normalize();
			auto yAxis = xAxis.Cross(Vector3<T>() - zAxis).Normalize();

			Math::Matrix<T, 4, 4> matrix(
					xAxis.X(), xAxis.Y(), xAxis.Z(), 0.0,
					yAxis.X(), yAxis.Y(), yAxis.Z(), 0.0,
					zAxis.X(), zAxis.Y(), zAxis.Z(), 0.0,
					0.0, 0.0, 0.0, 1.0
			);

			matrix = matrix.CalculateInverse();
			matrix.Get(0, 3) = 0.0;
			matrix.Get(1, 3) = 0.0;
			matrix.Get(2, 3) = 0.0;

			_directions[0] = Vector3<T>(-0.7, 0.7, 1.0)
					.Normalize()
					.Transform(matrix);

			_directions[1] = Vector3<T>(0.7, 0.7, 1.0)
					.Normalize()
					.Transform(matrix);

			_directions[2] = Vector3<T>(0.7, -0.7, 1.0)
					.Normalize()
					.Transform(matrix);

			_directions[3] = Vector3<T>(-0.7, -0.7, 1)
					.Normalize()
					.Transform(matrix);
		}

		Camera(const Camera<T>&) = default;
		Camera& operator=(const Camera<T>&) = default;
		Camera(Camera<T>&&) noexcept = default;
		Camera& operator=(Camera<T>&&) noexcept = default;
		~Camera() = default;
	private:
		Vector3<T> _origin;
		Vector3<T> _directions[4];

		void GenerateRayPair(T y, Ray<T>& a, Ray<T>& b) const
		{
			a = { _origin, (_directions[0] * y) + (_directions[3] * (1.0 - y)) };
			b = { _origin, (_directions[1] * y) + (_directions[2] * (1.0 - y)) };
		}
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_CAMERA_HPP
