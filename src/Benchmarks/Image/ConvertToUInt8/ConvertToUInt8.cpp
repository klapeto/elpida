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
 * ConvertToUInt8.cpp
 *
 *  Created on: 12 Ιουν 2018
 *      Author: klapeto
 */

#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8.hpp"

#include <Elpida/Utilities/Imaging/Image.hpp>
#include <Elpida/Engine/Data/ActiveTaskData.hpp>

namespace Elpida
{

	ConvertToUInt8::ConvertToUInt8(const TaskSpecification& specification, const ProcessorNode& processorToRun)
		: ImageTaskBase(specification, processorToRun), _outputData(nullptr), _outputImage(nullptr),
		  _inputImage(nullptr)
	{

	}

	void ConvertToUInt8::execute()
	{
		std::size_t size = _outputImage->getTotalSize();
		Pixel<PixelInt>* converted = _outputImage->getData();
		Pixel<PixelFloat>* source = _inputImage->getData();
		for (std::size_t i = 0; i < size; ++i)
		{
			converted[i].R = source[i].R * 255;
			converted[i].G = source[i].G * 255;
			converted[i].B = source[i].B * 255;
			converted[i].A = source[i].A * 255;
		}

	}
	void ConvertToUInt8::prepareImpl()
	{
		const auto& input = getInput();
		const auto& properties = getImageProperties(input);

		_inputImage = new Image<PixelFloat>(*input.getTaskData(), properties.width, properties.height);
		_outputData = new ActiveTaskData(properties.width * properties.height * 4 * sizeof(PixelInt), _processorToRun);
		_outputImage = new Image<PixelInt>(*_outputData, properties.width, properties.height);

	}

	TaskDataDto ConvertToUInt8::finalizeAndGetOutputData()
	{
		return TaskDataDto(*_outputData, getInput().getDefinedProperties());
	}

	double ConvertToUInt8::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _inputImage->getTotalSize();
	}

	ConvertToUInt8::~ConvertToUInt8()
	{
		delete _inputImage;
		delete _outputImage;
	}

} /* namespace Elpida */
