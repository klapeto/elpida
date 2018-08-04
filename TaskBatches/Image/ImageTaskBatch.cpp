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
 * ImageTaskBatch.cpp
 *
 *  Created on: 13 Μαρ 2018
 *      Author: klapeto
 */

#include "ImageTaskBatch.hpp"

#include "PngEncoding.hpp"
#include "PngDecoding.hpp"
#include "ConvertToFloat.hpp"
#include "ConvertToUInt8.hpp"
#include "FloydSteinberg.hpp"
#include "GrayscaleAverage.hpp"
#include "Elpida/Utilities/CommandParser.hpp"
#include "TaskBatches/General/ReadFile.hpp"
#include "TaskBatches/General/WriteFile.hpp"

namespace Elpida
{

	ImageTaskBatch::ImageTaskBatch()
			: TaskBatch("ImageTasks")
	{
	}

	ImageTaskBatch::~ImageTaskBatch()
	{
		destroyTasks();
	}

	void ImageTaskBatch::reconfigure(const String& inputData)
	{
		CommandParser parser;
		parser.parseCommand(inputData);

		_inputFile = parser.getArgument("i");
		_outputFile = parser.getArgument("o");

		if (_inputFile.size() > 0 && _outputFile.size() > 0)
		{
			destroyTasks();
			createTasks();
		}
		else
		{
			throw ElpidaException("ImageTaskBatch", "Attempted to reconfigure batch with either input or output file empty");
		}
	}

	void ImageTaskBatch::createTasks()
	{
		auto readFile = new ReadFile(_inputFile);
		auto decoding = new PngDecoding(readFile->getFile().getData(), readFile->getFile().getSize());
		auto floatConvert = new ConvertToFloat<RawData>(decoding->getImage());
		auto grayScale = new GrayscaleAverage<Float32>(floatConvert->getImage(), floatConvert->getImage());
		auto floydSteinberg = new FloydSteinberg<Float32>(floatConvert->getImage(), floatConvert->getImage(), 0.5f);
		auto intConvert = new ConvertToUInt8<Float32>(floatConvert->getImage());
		auto encoding = new PngEncoding(intConvert->getImage());
		auto writeFile = new WriteFile(encoding->getEncodedData(), encoding->getEncodedDataSize(), _outputFile);

		_tasks.push_back(readFile);
		_tasks.push_back(decoding);
		_tasks.push_back(floatConvert);
		_tasks.push_back(grayScale);
		_tasks.push_back(floydSteinberg);
		_tasks.push_back(intConvert);
		_tasks.push_back(encoding);
		_tasks.push_back(writeFile);
	}

} /* namespace Elpida */
