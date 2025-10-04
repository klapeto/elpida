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

#ifndef ELPIDA_SCENECREATOR_HPP
#define ELPIDA_SCENECREATOR_HPP

#include "Elpida/Graphics/Scene.hpp"
#include "Elpida/Graphics/Triangle.hpp"
#include "Elpida/Graphics/Vector3.hpp"
#include "Elpida/Graphics/Color.hpp"
#include "Elpida/Graphics/ShaderColor.hpp"

#include <cmath>

namespace Elpida
{
	using namespace Graphics;

	class SceneCreator
	{
	public:


		template<typename T>
		static Scene<T> GenerateScene()
		{
			Scene<T> scene;

			auto tfl = Vector3<T>(-10, 10, -10);
			auto tfr = Vector3<T>(10, 10, -10);
			auto tbl = Vector3<T>(-10, 10, 10);
			auto tbr = Vector3<T>(10, 10, 10);
			auto bfl = Vector3<T>(-10, -10, -10);
			auto bfr = Vector3<T>(10, -10, -10);
			auto bbl = Vector3<T>(-10, -10, 10);
			auto bbr = Vector3<T>(10, -10, 10);

			auto cubeShader = [](auto p)
			{ return ShaderColor<T>(0.7, 0.7, 0.7, 1.0, 0.0); };
			// cube

			//front
			scene.AddTriangle({ tfl, tfr, bfr, cubeShader });
			scene.AddTriangle({ tfl, bfr, bfl, cubeShader });

			//back
			scene.AddTriangle({ tbl, tbr, bbr, cubeShader });
			scene.AddTriangle({ tbl, bbr, bbl, cubeShader });

			//left
			scene.AddTriangle({ tbl, tfl, bbl, cubeShader });
			scene.AddTriangle({ tfl, bfl, bbl, cubeShader });

			//left
			scene.AddTriangle({ tbr, tfr, bbr, cubeShader });
			scene.AddTriangle({ tfr, bfr, bbr, cubeShader });

			//top
			scene.AddTriangle({ tbl, tbr, tfr, cubeShader });
			scene.AddTriangle({ tbl, tfr, tfl, cubeShader });

			//bottom
			scene.AddTriangle({ bbl, bbr, bfr, cubeShader });
			scene.AddTriangle({ bbl, bfr, bfl, cubeShader });


			auto ffl = Vector3<T>(-1000.0, -30.0, -1000.0);
			auto ffr = Vector3<T>(1000.0, -30.0, -1000.0);
			auto fbl = Vector3<T>(-1000.0, -30.0, 1000.0);
			auto fbr = Vector3<T>(1000.0, -30.0, 1000.0);

			// floor

			auto floorShader = [](auto p)
			{
				T x =std::fmod(p.X() / 32.0, 2.0);
				x+=2;
				x = std::fmod(x, 2.0);

				T z = p.Z() / 32.0 + 0.3;
				z = std::fmod(z, 2.0);
				z+=2;
				z = std::fmod(z, 2.0);

				if (x < 1.0 != z < 1.0)
				{
					return ShaderColor<T>{ 0.4, 0.4, 0.4, 1.0, 1.0 };
				}
				else
				{
					return ShaderColor<T>{ 0.0, 0.4, 0.0, 1.0 };
				}
			};

			scene.AddTriangle({ fbl, fbr, ffr, floorShader });
			scene.AddTriangle({ fbl, ffr, ffl, floorShader });

			scene.AddLight({
					{ 20.0, 38.0, -22.0 },
					{ 0.7, 0.3, 0.3 }
			});

			scene.AddLight({
					{ -23.0, 40.0, 17.0 },
					{ 0.7, 0.3, 0.3 }
			});

			scene.AddLight({
					{ 23.0, 20.0, 17.0 },
					{ 0.7, 0.7, 0.7 }
			});

			scene.SetAmbient({ 0.1, 0.1, 0.1 });

			return scene;
		}
	};

} // Elpida

#endif //ELPIDA_SCENECREATOR_HPP
