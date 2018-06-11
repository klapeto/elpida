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
 * ImageTaskBatch.cpp
 *
 *  Created on: 13 Μαρ 2018
 *      Author: klapeto
 */

#include "Tasks/Image/ImageTaskBatch.hpp"
#include "Tasks/Image/GrayscaleAverage.hpp"
#include "Tasks/Image/PngDecoding.hpp"
#include "Tasks/Image/PngEncoding.hpp"
#include "Tasks/General/ReadFile.hpp"
#include "Tasks/General/WriteFile.hpp"

namespace Elpida
{

	ImageTaskBatch::ImageTaskBatch(const std::string& inputImage, const std::string& outputImage) :
			TaskBatch("Image Tasks for: " + inputImage), _inputFile(inputImage), _outputFile(outputImage)
	{
		reconfigure();
	}

	ImageTaskBatch::~ImageTaskBatch()
	{
		destroyTasks();
	}

	void ImageTaskBatch::reconfigure()
	{
		destroyTasks();
		createTasks();
	}

	void ImageTaskBatch::createTasks()
	{
		auto readFile = new ReadFile(_inputFile);
		auto decoding = new PngDecoding(readFile->getFile().getData(), readFile->getFile().getSize());
		auto grayScale = new GrayscaleAverage<unsigned char>(decoding->getImage(), decoding->getImage());
		auto encoding = new PngEncoding(decoding->getImage());
		auto writeFile = new WriteFile(encoding->getEncodedData(), encoding->getEncodedDataSize(), _outputFile);

		_tasks.push_back(readFile);
		_tasks.push_back(decoding);
		_tasks.push_back(grayScale);
		_tasks.push_back(encoding);
		_tasks.push_back(writeFile);
	}

} /* namespace Elpida */
