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
 * Grayscale.cpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverage.hpp"

#include "Benchmarks/Image/Image.hpp"

namespace Elpida
{

	GrayscaleAverage::GrayscaleAverage(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: ImageTaskBase(specification, processorToRun, serviceProvider, iterationsToRun)
	{

	}

	void GrayscaleAverage::run()
	{
		std::size_t size = _inputImage->getTotalSize();
		Pixel<PixelFloat>* sourceData = _inputImage->getData();
		for (std::size_t i = 0; i < size; ++i)
		{
			PixelFloat newColor = (sourceData[i].R + sourceData[i].G + sourceData[i].B) / 3.0;
			sourceData[i].R = newColor;
			sourceData[i].G = newColor;
			sourceData[i].B = newColor;
			sourceData[i].A = 1.0;
		}
	}

	void GrayscaleAverage::prepareImpl()
	{
		const auto& input = getInput();
		auto properties = getImageProperties(input);

		_inputImage = std::make_unique<Image<PixelFloat>>(*input.getTaskData(), properties.width, properties.height);
	}

	std::optional<TaskDataDto> GrayscaleAverage::finalizeAndGetOutputData()
	{
		_inputImage.reset();

		return propagateInput();
	}

	double GrayscaleAverage::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _inputImage->getTotalSize();
	}

} /* namespace Elpida */
