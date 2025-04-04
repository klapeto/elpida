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
// Created by klapeto on 15/9/2024.
//

#ifndef ELPIDA_NBODYSYSTEM_HPP
#define ELPIDA_NBODYSYSTEM_HPP

#include "Body.hpp"

#include <vector>
#include <cmath>

namespace Elpida
{

	template<typename T>
	class NBodySystem
	{
	public:

		T Energy() const
		{
			T e = 0.0;
			auto size = _bodies.size();
			for (std::size_t i = 0; i < size; i++)
			{
				auto& bodyI = _bodies[i];

				e += 0.5 * bodyI.GetMass() *
					 (bodyI.GetVx() * bodyI.GetVx()
					  + bodyI.GetVy() * bodyI.GetVy()
					  + bodyI.GetVz() * bodyI.GetVz());

				for (std::size_t j = i + 1; j < size; j++)
				{
					auto& bodyJ = _bodies[j];
					auto dx = bodyI.GetX() - bodyJ.GetX();
					auto dy = bodyI.GetY() - bodyJ.GetY();
					auto dz = bodyI.GetZ() - bodyJ.GetZ();

					auto distance = std::sqrt(dx * dx + dy * dy + dz * dz);
					e -= (bodyI.GetMass() * bodyJ.GetMass()) / distance;
				}
			}
			return e;

		}

		void Advance(T dt)
		{
			auto size = _bodies.size();

			for (std::size_t i = 0; i < size; i++)
			{
				auto& bodyI = _bodies[i];
				for (std::size_t j = i + 1; j < size; j++)
				{
					auto& bodyJ = _bodies[j];

					auto dx = bodyI.GetX() - bodyJ.GetX();
					auto dy = bodyI.GetY() - bodyJ.GetY();
					auto dz = bodyI.GetZ() - bodyJ.GetZ();

					auto distance = std::sqrt(dx * dx + dy * dy + dz * dz);
					auto mag = dt / (distance * distance * distance);

					bodyI.GetVx() -= dx * bodyJ.GetMass() * mag;
					bodyI.GetVy() -= dy * bodyJ.GetMass() * mag;
					bodyI.GetVz() -= dz * bodyJ.GetMass() * mag;

					bodyJ.GetVx() += dx * bodyI.GetMass() * mag;
					bodyJ.GetVy() += dy * bodyI.GetMass() * mag;
					bodyJ.GetVz() += dz * bodyI.GetMass() * mag;
				}
			}

			for (std::size_t i = 0; i < size; i++)
			{
				auto& body = _bodies[i];
				body.GetX() += dt * body.GetVx();
				body.GetY() += dt * body.GetVy();
				body.GetZ() += dt * body.GetVz();
			}
		}

		NBodySystem() = default;

		explicit NBodySystem(std::vector<Body<T>>&& bodies)
				:_bodies(std::move(bodies))
		{
			T px = 0.0;
			T py = 0.0;
			T pz = 0.0;

			for (auto& body : _bodies)
			{
				auto m = body.GetMass();
				px += body.GetVx() * m;
				py += body.GetVy() * m;
				pz += body.GetVz() * m;
			}
			_bodies[0].OffsetMomentum(px, py, pz);
		}

	private:
		std::vector<Body<T>> _bodies;
	};

} // Elpida

#endif //ELPIDA_NBODYSYSTEM_HPP
