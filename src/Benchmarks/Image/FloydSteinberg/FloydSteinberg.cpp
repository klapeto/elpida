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
 * FloydSteinberg.cpp
 *
 *  Created on: 11 Ιουν 2018
 *      Author: klapeto
 */

#include "Benchmarks/Image/FloydSteinberg/FloydSteinberg.hpp"

#include "Benchmarks/Image/Config.hpp"
#include "Benchmarks/Image/Image.hpp"

namespace Elpida
{

	FloydSteinberg::FloydSteinberg(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		double threshold,
		size_t iterationsToRun)
		: ImageTaskBase(specification, processorToRun, serviceProvider, iterationsToRun),
		_threshold(threshold)
	{

	}

	void FloydSteinberg::run()
	{
		std::size_t width = _inputImage->getWidth();
		std::size_t height = _inputImage->getHeight();
		Pixel<PixelFloat>* data = _inputImage->getData();
		for (std::size_t y = 0; y < height; ++y)
		{
			for (std::size_t x = 0; x < width; ++x)
			{
				std::size_t index = (y * width) + x;

				PixelFloat newColorR = data[index].R < _threshold ? 0.0 : 1.0;
				PixelFloat newColorG = data[index].G < _threshold ? 0.0 : 1.0;
				PixelFloat newColorB = data[index].B < _threshold ? 0.0 : 1.0;

				PixelFloat quantizationErrorR = data[index].R - newColorR;
				PixelFloat quantizationErrorG = data[index].G - newColorG;
				PixelFloat quantizationErrorB = data[index].B - newColorB;

				data[index].R = newColorR;
				data[index].G = newColorG;
				data[index].B = newColorB;
				data[index].A = 1.0;

				if (x + 1u < width)
				{
					data[index + 1].R += (quantizationErrorR * 7.0) / 16.0;    // [x + 1, y]
					data[index + 1].G += (quantizationErrorG * 7.0) / 16.0;
					data[index + 1].B += (quantizationErrorB * 7.0) / 16.0;
					data[index + 1].A = 1.0;
				}

				if (y + 1u < height)
				{
					if (x - 1 >= 0)
					{
						data[index + width - 1].R += (quantizationErrorR * 3.0) / 16.0;    // [x - 1, y + 1]
						data[index + width - 1].G += (quantizationErrorG * 3.0) / 16.0;
						data[index + width - 1].B += (quantizationErrorB * 3.0) / 16.0;
						data[index + width - 1].A = 1.0;
					}

					data[index + width].R += (quantizationErrorR * 5.0) / 16.0;    // [x, y + 1]
					data[index + width].G += (quantizationErrorG * 5.0) / 16.0;
					data[index + width].B += (quantizationErrorB * 5.0) / 16.0;
					data[index + width].A = 1.0;

					if (x + 1 < width)
					{
						data[index + width + 1].R += quantizationErrorR / 16.0;    // [x + 1, y + 1]
						data[index + width + 1].G += quantizationErrorG / 16.0;
						data[index + width + 1].B += quantizationErrorB / 16.0;
						data[index + width + 1].A = 1.0;
					}
				}
			}
		}
	}

	void FloydSteinberg::prepareImpl()
	{
		const auto& input = getInput();
		auto properties = getImageProperties(input);
		_inputImage = std::make_unique<Image<PixelFloat>>(*input.getTaskData(), properties.width, properties.height);
	}

	std::optional<TaskDataDto> FloydSteinberg::finalizeAndGetOutputData()
	{
		_inputImage.reset();

		return propagateInput();
	}

	double FloydSteinberg::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _inputImage->getTotalSize();
	}

} /* namespace Elpida */
