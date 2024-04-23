//
// Created by klapeto on 23/4/2024.
//


#include <fstream>
#include <iostream>

#include "Elpida/Svg/SvgColorConverter.hpp"
#include "Elpida/Core/ThreadPool/ThreadPool.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "png.h"
#include "Elpida/Svg/SvgRasterizer.hpp"
#include "Elpida/Svg/SvgDocument.hpp"
#include "Elpida/Xml/XmlParser.hpp"
#include "Elpida/Svg/SvgCalculatedDocument.hpp"

using namespace Elpida;

int main(int argC, char** argV)
{

	std::ifstream file("/media/klapeto/Αρχεία/Code/elpida/apps/Application/Assets/Art_Bot.svg",
			std::ifstream::binary | std::ifstream::in);

	file.seekg(0, std::ifstream::end);
	auto size = file.tellg();
	file.seekg(0, std::ifstream::beg);

	auto inputData = std::unique_ptr<char[]>(new char[size]);

	file.read(inputData.get(), size);


	auto element = XmlParser::Parse(inputData.get(), size);

	SvgDocument svgDocument(element);

	auto calculated = SvgCalculatedDocument(svgDocument, 1.0);

	SvgRasterizer rasterizer;

	ThreadPool tp(32 * 20, 32 * 20);

	SvgBackDrop backDrop;

	auto a = std::chrono::high_resolution_clock::now();
	backDrop = rasterizer.RasterizeMultiThreaded(calculated, tp, 16);
	//auto backDrop = rasterizer.Rasterize(calculated, 16);
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