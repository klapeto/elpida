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

#ifndef ELPIDA_GRAPHICS_SCENE_HPP
#define ELPIDA_GRAPHICS_SCENE_HPP

#include <vector>
#include <limits>
#include <optional>

#include "Elpida/Graphics/Light.hpp"
#include "Elpida/Graphics/Color.hpp"
#include "Elpida/Graphics/Vector3.hpp"
#include "Elpida/Graphics/Triangle.hpp"

namespace Elpida::Graphics
{
	template<typename T>
	class Scene
	{
	public:

		Color<T> Intersect(const Vector3<T>& origin, const Vector3<T>& direction, T near = 0.001,
				T far = std::numeric_limits<T>::max()) const
		{
			const Triangle<T>* closest = nullptr;

			for (auto& triangle : _triangles)
			{
				auto d = triangle.IntersectsRay(origin, direction, near, far);
				if (d < 0.0 || d > far || d < near) continue;
				far = d;
				closest = &triangle;
			}

			if (closest == nullptr) return _background;

			auto normal = closest->GetNormal();
			auto hit = origin + (direction * far);

			if (direction.DotProduct(normal) > 0.0)
			{
				normal = { -normal.X(), -normal.Y(), -normal.Z() };
			}

			auto color = closest->GetShader()(hit);

			std::optional<Color<T>> reflected;
			if (color.GetReflection() > 0.001)
			{
				auto reflection = (normal * (-2.0 * direction.DotProduct(normal))) + direction;
				reflected = Intersect(hit, reflection, 0.0001, 1000000);
				if (color.GetReflection() >= 0.999999)
				{
					return reflected.value();
				}
			}

			auto l = _ambient;
			for (auto& light : _lights)
			{
				auto toLight = light.GetLocation() - hit;
				auto distance = toLight.Length();
				toLight *= 1.0 / distance;
				distance -= 0.0001;

				if (IsBlocked(hit, toLight, distance)) continue;

				auto nl = normal.DotProduct(toLight);
				if (nl > 0.0)
				{
					l += light.GetColor() * nl;
				}
			}
			l = l * color;
			if (reflected.has_value())
			{
				l *= 1.0 - color.GetReflection();
				l = l + (reflected.value() * color.GetReflection());
			}

			return l;
		}

		void SetAmbient(const Color<T>& color)
		{
			_ambient = color;
		}

		void AddLight(Light<T>&& light)
		{
			_lights.push_back(std::move(light));
		}

		void AddTriangle(const Triangle<T>& triangle)
		{
			_triangles.push_back(triangle);
		}

		Scene()
				:_background(0.8, 0.8, 1.0)
		{

		}

	private:
		std::vector<Triangle<T>> _triangles;
		std::vector<Light<T>> _lights;
		Color<T> _ambient;
		Color<T> _background;

		bool IsBlocked(const Vector3<T>& origin, const Vector3<T>& direction, T far) const
		{
			T near = 0.0001;
			const Triangle<T>* closest = nullptr;
			for (auto& triangle : _triangles)
			{
				auto d = triangle.IntersectsRay(origin, direction, near, far);
				if (d < 0.0 || d > far || d < near) continue;
				return true;
			}

			return false;
		}
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_SCENE_HPP
