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
 * ConvertToFloat.cpp
 *
 *  Created on: 12 Ιουν 2018
 *      Author: klapeto
 */

#include "Benchmarks/Image/ConvertToFloat/ConvertToFloat.hpp"

#include <Elpida/Engine/Data/ActiveTaskData.hpp>
#include <Elpida/Utilities/Imaging/Image.hpp>

namespace Elpida
{

	ConvertToFloat::ConvertToFloat(const TaskSpecification& specification, const ProcessorNode& processorToRun)
		: ImageTaskBase(specification, processorToRun), _outputData(nullptr), _inputImage(nullptr),
		  _outputImage(nullptr)
	{

	}

	void ConvertToFloat::prepareImpl()
	{
		const auto& input = getInput();

		auto properties = getImageProperties(input);

		_inputImage = new Image<PixelInt>(*input.getTaskData(), properties.width, properties.height);
		_outputData = new ActiveTaskData(properties.width * properties.height * 4 * sizeof(PixelFloat), _processorToRun);
		_outputImage = new Image<PixelFloat>(*_outputData, properties.width, properties.height);

	}

	TaskDataDto ConvertToFloat::finalizeAndGetOutputData()
	{
		return TaskDataDto(*_outputData, getInput().getDefinedProperties());
	}

	double ConvertToFloat::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _outputImage->getTotalSize();
	}

	void ConvertToFloat::execute()
	{
		auto size = _inputImage->getTotalSize();
		auto inputData = _inputImage->getData();
		auto outputData = _outputImage->getData();
		for (std::size_t i = 0; i < size; ++i)
		{
			outputData[i].R = inputData[i].R / (PixelFloat)255.0;
			outputData[i].G = inputData[i].G / (PixelFloat)255.0;
			outputData[i].B = inputData[i].B / (PixelFloat)255.0;
			outputData[i].A = inputData[i].A / (PixelFloat)255.0;
		}
	}

	ConvertToFloat::~ConvertToFloat()
	{
		delete _inputImage;
		delete _outputImage;
	}
} /* namespace Elpida */
