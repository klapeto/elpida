/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * Pixel.hpp
 *
 *  Created on: 13 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_IMAGING_PIXEL_HPP_
#define ELPIDA_UTILITIES_IMAGING_PIXEL_HPP_

#include <type_traits>

namespace Elpida
{

	template<typename T>
	class Pixel
	{
		static_assert(std::is_arithmetic<T>::value, "Image requires an arithmetic template parameter");
	public:

		T R;
		T G;
		T B;
		T A;

		inline Pixel()
		{

		}

		inline Pixel(T r, T g, T b, T a)
			: R(r), G(g), B(b), A(a)
		{

		}

		inline ~Pixel()
		{

		}

		inline Pixel(Pixel&&) = default;
		inline Pixel(const Pixel&) = default;
		inline Pixel& operator=(Pixel&&) = default;
		inline Pixel& operator=(const Pixel&) = default;
	};

} /* namespace Elpida */

#endif /* ELPIDA_UTILITIES_IMAGING_PIXEL_HPP_ */
