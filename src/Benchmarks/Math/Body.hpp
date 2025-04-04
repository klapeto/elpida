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

#ifndef ELPIDA_BODY_HPP
#define ELPIDA_BODY_HPP

namespace Elpida
{

	template<typename T>
	class Body
	{
	public:
		static constexpr T Pi = 3.1415926535897932384626433;
		static constexpr T SolarMass = 4 * Pi * Pi;
		static constexpr T DaysPerYear = 365.24;

		T& GetX()
		{
			return _x;
		}

		T& GetY()
		{
			return _y;
		}

		T& GetZ()
		{
			return _z;
		}

		T& GetVx()
		{
			return _vx;
		}

		T& GetVy()
		{
			return _vy;
		}

		T& GetVz()
		{
			return _vz;
		}

		T& GetMass()
		{
			return _mass;
		}

		T GetX() const
		{
			return _x;
		}

		T GetY() const
		{
			return _y;
		}

		T GetZ() const
		{
			return _z;
		}

		T GetVx() const
		{
			return _vx;
		}

		T GetVy() const
		{
			return _vy;
		}

		T GetVz() const
		{
			return _vz;
		}

		T GetMass() const
		{
			return _mass;
		}

		Body<T>& OffsetMomentum(T px, T py, T pz)
		{
			_vx = -px / SolarMass;
			_vy = -py / SolarMass;
			_vz = -pz / SolarMass;
			return *this;
		}

		Body(T x, T y, T z, T vx, T vy, T vz, T mass)
				:_x(x), _y(y), _z(z), _vx(vx), _vy(vy), _vz(vz), _mass(mass)
		{
		}

	private:
		T _x;
		T _y;
		T _z;
		T _vx;
		T _vy;
		T _vz;
		T _mass;


	};

} // Elpida

#endif //ELPIDA_BODY_HPP
