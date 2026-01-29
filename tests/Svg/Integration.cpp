/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 23/4/2024.
//

#include <fstream>
#include <iostream>

#include "Elpida/Svg/SvgColorConverter.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "png.h"
#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgDirectRasterizer.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Xml/XmlParser.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"

using namespace Elpida;

#define NON_CONFORMING

int main(int argC, char** argV)
{
	std::ifstream file("/home/klapeto/Λήψεις/Art_Bot.svg",
			std::ifstream::binary | std::ifstream::in);

	file.seekg(0, std::ifstream::end);
	auto size = file.tellg();
	file.seekg(0, std::ifstream::beg);

	auto inputData = std::unique_ptr<char[]>(new char[size]);

	file.read(inputData.get(), size);


	auto element = XmlParser::Parse(inputData.get(), size);

	SvgDocument svgDocument(element);

	auto calculated = SvgCalculatedDocument(svgDocument, 1.0);

#ifdef NON_CONFORMING
	SvgDirectRasterizer rasterizer;
#else
	SvgRasterizer rasterizer;
#endif

#ifdef NON_CONFORMING
	SvgBackDrop backDrop(std::ceil(calculated.GetViewPort().GetWidth()), std::ceil(calculated.GetViewPort().GetHeight()));
	SvgBackDrop tempBackDrop(std::ceil(calculated.GetViewPort().GetWidth()), std::ceil(calculated.GetViewPort().GetHeight()));
#else
	SvgBackDrop backDrop;
#endif

	auto a = std::chrono::high_resolution_clock::now();
#ifdef NON_CONFORMING
	rasterizer.Rasterize(calculated, backDrop, tempBackDrop, 32);
#else
	backDrop = rasterizer.Rasterize(calculated, 16);
#endif
	auto b = std::chrono::high_resolution_clock::now();

	std::cout << "Took: " << std::chrono::duration_cast<std::chrono::duration<double>>(b - a).count() << std::endl;

	std::size_t width = backDrop.GetWidth();
	std::size_t height = backDrop.GetHeight();
	auto bitmapData = SvgColorConverter::Convert<unsigned char>(backDrop.GetColorData());
	{
		png_image image{};
		image.version = PNG_IMAGE_VERSION;

		std::size_t outputSize;

		image.width = width;
		image.height = height;
		image.format = PNG_FORMAT_RGBA;

		if (png_image_write_to_memory(&image, nullptr, &outputSize, 0, bitmapData.get(), 0, nullptr))
		{
			auto outputData = std::unique_ptr<char[]>(new char[outputSize]);

			if (!png_image_write_to_memory(&image,
					outputData.get(),
					&outputSize,
					0,
					bitmapData.get(),
					0,
					nullptr))
			{
				throw ElpidaException("Failed to encode image: ", image.message);
			}

			std::ofstream outputFile("/home/klapeto/σχεδίαση.out.png",
					std::ofstream::binary | std::ofstream::out | std::ofstream::trunc);

			outputFile.write(outputData.get(), outputSize);
		}
		else
		{
			throw ElpidaException("Failed to encode image: ", image.message);
		}
	}

	return 0;
}