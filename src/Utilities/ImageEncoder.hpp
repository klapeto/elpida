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
 * PngLoader.hpp
 *
 *  Created on: 15 Μαρ 2018
 *      Author: klapeto
 */

#ifndef SRC_UTILITIES_IMAGEENCODER_HPP_
#define SRC_UTILITIES_IMAGEENCODER_HPP_

#include <cstddef>
#include <string>

namespace Elpida
{
	class ImageEncoder
	{
		public:

			struct ImageDecodeInfo
			{
					uint8_t* data;
					size_t width;
					size_t height;
					int pixelSize;
			};

			struct ImageEncodeInfo
			{
					uint8_t* data;
					size_t dataSize;
			};

			virtual ImageDecodeInfo decode(uint8_t* data, size_t size) = 0;
			virtual ImageEncodeInfo encode(size_t imageWidth, size_t imageHeight, uint8_t* inputData, size_t inputSize) = 0;

			ImageEncoder()
			{

			}
			virtual ~ImageEncoder()
			{

			}

			ImageEncoder(ImageEncoder&&) = default;
			ImageEncoder(const ImageEncoder&) = default;
			ImageEncoder& operator=(ImageEncoder&&) = default;
			ImageEncoder& operator=(const ImageEncoder&) = default;
	};

} /* namespace Elpida */

#endif /* SRC_UTILITIES_IMAGEENCODER_HPP_ */
