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

#ifndef ELPIDA_GRAPHICS_LIGHT_HPP
#define ELPIDA_GRAPHICS_LIGHT_HPP

#include "Elpida/Graphics/Color.hpp"
#include "Elpida/Graphics/Vector3.hpp"

namespace Elpida::Graphics
{

	template<typename T>
	class Light
	{
	public:

		const Vector3<T>& GetLocation() const
		{
			return _location;
		}

		const Color<T>& GetColor() const
		{
			return _color;
		}

		Light() = default;

		Light(const Vector3<T>& location, const Color<T>& color)
				:_location(location), _color(color)
		{
		}

	private:
		Vector3<T> _location;
		Color<T> _color;
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_LIGHT_HPP
