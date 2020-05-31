/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#ifndef ELPIDA_UTILITIES_IMAGING_IMAGE_HPP_
#define ELPIDA_UTILITIES_IMAGING_IMAGE_HPP_

#include <cstddef>
#include "Elpida/Config.hpp"

#include "Elpida/ElpidaException.hpp"
#include "Pixel.hpp"
#include "Elpida/Utilities/RawData.hpp"

namespace Elpida
{


	template<typename T = unsigned char>
	class Image
	{
	public:

		[[nodiscard]] std::size_t getTotalSize() const
		{
			return _width * _height;
		}

		[[nodiscard]] std::size_t getTotalSizeInBytes() const
		{
			return _width * _height * sizeof(Pixel<T>);
		}

		Pixel<T>* getData() const
		{
			return _data;
		}

		[[nodiscard]] std::size_t getHeight() const
		{
			return _height;
		}

		[[nodiscard]] std::size_t getWidth() const
		{
			return _width;
		}

		template<typename oT>
		inline bool isCompatibleWith(const Image<oT>& other) const
		{
			return other._width == this->_width && other._height == this->_height;
		}

		inline Pixel<T>& getPixel(std::size_t x, std::size_t y)
		{
			return _data[(y * _width) + x];
		}

		[[nodiscard]] const RawData& getRawData() const
		{
			return _rawData;
		}

		Image(const RawData& rawData, std::size_t width, std::size_t height)
			: _rawData(rawData), _width(width), _height(height)
		{
			if (width > 0 && height > 0)
			{
				auto actualSize = width * height * 4 *  sizeof(T);
				if (actualSize != rawData.getSize())
				{
					throw ElpidaException(FUNCTION_NAME, "Data size does not match the image size!");
				}

				_data = (Pixel<T>*)rawData.getData();
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME, "Size must be at least 1x1");
			}
		}
		~Image() = default;
	private:
		const RawData& _rawData;
		Pixel<T>* _data;
		std::size_t _width;
		std::size_t _height;
	};

}
/* namespace Elpida */

#endif /* ELPIDA_UTILITIES_IMAGING_IMAGE_HPP_ */
