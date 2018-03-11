/**************************************************************************
 *   elpida - CPU benchmark tool
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
 * Image.hpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_TASKS_IMAGE_IMAGE_HPP_
#define SRC_TASKS_IMAGE_IMAGE_HPP_

#include <stddef.h>
#include <type_traits>

namespace Elpida
{

	template<typename T>
	class Image
	{
			static_assert(std::is_arithmetic<T>::value, "Image requires an arithmetic template parameter");
		public:

			inline T& getPixel(size_t x, size_t y)
			{
				return _data[(y * _width) + y];
			}

			Image();
			Image(T* data, size_t width, size_t height, bool copyData = false);
			~Image();
		private:
			T* _data;
			size_t _width;
			size_t _height;
			bool _dataIsCopied;
	};

} /* namespace Elpida */

#endif /* SRC_TASKS_IMAGE_IMAGE_HPP_ */
