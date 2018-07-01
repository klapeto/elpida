/*******************************************************************************
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
 *******************************************************************************/

/*
 * LibPngLoader.cpp
 *
 *  Created on: 15 Μαρ 2018
 *      Author: klapeto
 */

#include "Tasks/Image/Encoders/LibPngEncoder.hpp"

#include "../libs/libpng/png.h"

namespace Elpida
{

	ImageEncoder::ImageDecodeInfo LibPngEncoder::decode(uint8_t* data, size_t size)
	{
		png_image img;
		img.opaque = nullptr;
		img.version = PNG_IMAGE_VERSION;

		if (png_image_begin_read_from_memory(&img, data, size))
		{
			img.format = PNG_FORMAT_RGBA;

			auto data = new uint8_t[PNG_IMAGE_SIZE(img)];

			if (data == nullptr)
			{
				png_image_free(&img);
				return ImageEncoder::ImageDecodeInfo { nullptr, 0, 0, 0 };
			}
			png_image_finish_read(&img, nullptr, data, 0, nullptr);

			return ImageEncoder::ImageDecodeInfo { data, img.width, img.height, 4 };

		}
		png_image_free(&img);
		return ImageEncoder::ImageDecodeInfo { nullptr, 0, 0, 0 };
	}

	ImageEncoder::ImageEncodeInfo LibPngEncoder::encode(size_t imageWidth, size_t imageHeight, uint8_t* inputData,
			size_t inputSize)
	{
		png_image img;
		img.opaque = nullptr;
		img.version = PNG_IMAGE_VERSION;
		img.width = imageWidth;
		img.height = imageHeight;
		img.format = PNG_FORMAT_RGBA;

		size_t outputSize;

		if (png_image_write_to_memory(&img, nullptr, &outputSize, 0, inputData, 0, nullptr))
		{
			auto outputBuffer = new uint8_t[outputSize];
			if (outputBuffer == nullptr)
			{
				png_image_free(&img);
				return ImageEncoder::ImageEncodeInfo { nullptr, 0 };
			}
			if (png_image_write_to_memory(&img, outputBuffer, &outputSize, 0, inputData, 0, nullptr))
			{
				return ImageEncoder::ImageEncodeInfo { outputBuffer, outputSize };
			}
		}
		png_image_free(&img);
		return ImageEncoder::ImageEncodeInfo { nullptr, 0 };
	}

} /* namespace Elpida */

