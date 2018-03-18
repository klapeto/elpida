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
 * LibPngLoader.cpp
 *
 *  Created on: 15 Μαρ 2018
 *      Author: klapeto
 */

#include "Tasks/Image/Loaders/LibPngLoader.hpp"

#include "../libs/libpng/png.h"

namespace Elpida
{

	ImageLoader::ImageInfo LibPngLoader::loadToMemory(const std::string& path) const
	{

		png_image img;
		img.opaque = nullptr;
		img.version = PNG_IMAGE_VERSION;

		if (png_image_begin_read_from_file(&img, path.c_str()))
		{
			img.format = PNG_FORMAT_RGBA;

			auto data = new unsigned char[PNG_IMAGE_SIZE(img)];

			if (data == nullptr)
			{
				png_image_free(&img);
				return { 0, 0, 0, nullptr };
			}
			png_image_finish_read(&img, nullptr, data, 0, nullptr);

			ImageLoader::ImageInfo ret = { img.width, img.height, 4, data };

			return ret;

		}
		png_image_free(&img);
		return { 0, 0, 0, nullptr };
	}

	bool LibPngLoader::writeToFile(const std::string& path, const ImageInfo &image) const
	{
		png_image img;
		img.opaque = nullptr;
		img.version = PNG_IMAGE_VERSION;
		img.width = image.width;
		img.height = image.height;
		img.format = PNG_FORMAT_RGBA;

		return png_image_write_to_file(&img, path.c_str(), 0, image.data, 0, nullptr);
	}

} /* namespace Elpida */

