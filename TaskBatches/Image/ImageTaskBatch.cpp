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

#include "TaskBatches/Image/ImageTaskBatch.hpp"

#include "Elpida/Exceptions/ElpidaException.hpp"
#include "Elpida/Task.hpp"
#include "Elpida/Utilities/CommandParser.hpp"
#include "Elpida/Utilities/Image.hpp"
#include "Elpida/Utilities/MemoryFile.hpp"
#include "TaskBatches/General/ReadFile.hpp"
#include "TaskBatches/General/WriteFile.hpp"
#include "TaskBatches/Image/ConvertToFloat.hpp"
#include "TaskBatches/Image/ConvertToUInt8.hpp"
#include "TaskBatches/Image/FloydSteinberg.hpp"
#include "TaskBatches/Image/GrayscaleAverage.hpp"
#include "TaskBatches/Image/PngDecoding.hpp"
#include "TaskBatches/Image/PngEncoding.hpp"

namespace Elpida
{

	ImageTaskBatch::ImageTaskBatch()
			: TaskBatch("ImageTasks"), _outputEnabled(false)
	{
	}

	ImageTaskBatch::~ImageTaskBatch()
	{

	}

	void ImageTaskBatch::reconfigure(const std::string& inputData)
	{
		CommandParser parser;
		parser.parseCommand(inputData);

		_inputFile = parser.getArgument("i");
		_outputFile = parser.getArgument("o");

		_outputEnabled = _outputFile.size() > 0;
		if (_inputFile.size() == 0)
		{
			throw ElpidaException("ImageTaskBatch", "Attempted to reconfigure batch with either input or output file empty");
		}
	}

	void ImageTaskBatch::createTasks() const
	{
		auto readFile = new ReadFile(_inputFile);
		readFile->setToBeMeasured(false);
		auto decoding = new PngDecoding(readFile->getFile().getData(), readFile->getFile().getSize());
		auto floatConvert = new ConvertToFloat<unsigned char>(decoding->getImage());
		auto grayScale = new GrayscaleAverage<float>(floatConvert->getImage(), floatConvert->getImage());
		auto floydSteinberg = new FloydSteinberg<float>(floatConvert->getImage(), floatConvert->getImage(), 0.5f);
		auto intConvert = new ConvertToUInt8<float>(floatConvert->getImage());
		auto encoding = new PngEncoding(intConvert->getImage());

		addTask(readFile);
		addTask(decoding);
		addTask(floatConvert);
		addTask(grayScale);
		addTask(floydSteinberg);
		addTask(intConvert);
		addTask(encoding);

		if (_outputFile.size() > 0)
		{
			auto writeFile = new WriteFile(encoding->getEncodedData(), encoding->getEncodedDataSize(), _outputFile);
			writeFile->setToBeMeasured(false);
			addTask(writeFile);
		}

	}

} /* namespace Elpida */
