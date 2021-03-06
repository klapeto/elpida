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
 * PngWrite.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Benchmarks/Image/PngEncoding/PngEncoding.hpp"

#include "Benchmarks/Image/Image.hpp"
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Engine/Data/RawTaskData.hpp>

#include "png.h"

namespace Elpida
{
	PngEncoding::PngEncoding(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: ImageTaskBase(specification, processorToRun, serviceProvider, iterationsToRun),
		_outputData(nullptr),
		_inputImage(nullptr)
	{

	}

	void PngEncoding::run()
	{
		png_image img;
		img.opaque = nullptr;
		img.version = PNG_IMAGE_VERSION;
		img.width = _inputImage->getWidth();
		img.height = _inputImage->getHeight();
		img.format = PNG_FORMAT_RGBA;

		size_t outputSize;

		if (png_image_write_to_memory(&img, nullptr, &outputSize, 0, _inputImage->getRawData().getData(), 0, nullptr))
		{
			_outputData = std::make_unique<RawTaskData>(outputSize, _processorToRun);
			if (!png_image_write_to_memory(&img,
				_outputData->getData(),
				&outputSize,
				0,
				_inputImage->getRawData().getData(),
				0,
				nullptr))
			{
				std::string error(img.message);
				throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to encode image: ", error));
			}
		}
		else
		{
			std::string error(img.message);
			png_image_free(&img);
			throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to encode image: ", error));
		}
	}

	void PngEncoding::prepareImpl()
	{
		const auto& input = getInput();

		auto properties = getImageProperties(input);

		_inputImage = std::make_unique<Image<PixelInt>>(*input.getTaskData(), properties.width, properties.height);
	}

	std::optional<TaskDataDto> PngEncoding::finalizeAndGetOutputData()
	{
		_inputImage.reset();

		return TaskDataDto(std::move(_outputData));
	}

	double PngEncoding::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _inputImage->getTotalSize();
	}

} /* namespace Elpida */
