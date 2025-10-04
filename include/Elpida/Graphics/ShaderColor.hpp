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

#ifndef ELPIDA_GRAPHICS_SHADERCOLOR_HPP
#define ELPIDA_GRAPHICS_SHADERCOLOR_HPP

#include "Elpida/Graphics/Color.hpp"

namespace Elpida::Graphics
{

	template<typename T>
	class ShaderColor: public Color<T>
	{
	public:

		[[nodiscard]]
		T GetReflection() const
		{
			return _reflection;
		}

		ShaderColor()
				:Color<T>(), _reflection(0.0)
		{

		}

		ShaderColor(const T r, const T g, const T b, const T a)
				:Color<T>(r, g, b, a), _reflection(0.0)
		{
		}

		ShaderColor(const T r, const T g, const T b, const T a, T reflection)
				:Color<T>(r, g, b, a), _reflection(reflection)
		{
		}

	private:
		T _reflection;
	};

} // Elpida::Graphics

#endif //ELPIDA_GRAPHICS_SHADERCOLOR_HPP
