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
 * PngLoader.hpp
 *
 *  Created on: 15 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_UTILITIES_IMAGING_IMAGEENCODER_HPP_
#define ELPIDA_UTILITIES_IMAGING_IMAGEENCODER_HPP_

#include <cstddef>

namespace Elpida
{
	class ImageEncoder
	{
	public:

		struct ImageDecodeInfo
		{
			unsigned char* data;
			std::size_t width;
			std::size_t height;
			int pixelSize;
		};

		struct ImageEncodeInfo
		{
			unsigned char* data;
			std::size_t dataSize;
		};

		virtual ImageDecodeInfo decode(unsigned char* data, std::size_t size) = 0;
		virtual ImageEncodeInfo encode(std::size_t imageWidth, std::size_t imageHeight, unsigned char* inputData,
			std::size_t inputSize) = 0;

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

#endif /* ELPIDA_UTILITIES_IMAGING_IMAGEENCODER_HPP_ */
