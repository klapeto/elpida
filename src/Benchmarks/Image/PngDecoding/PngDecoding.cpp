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
 * PngLoad.cpp
 *
 *  Created on: 18 Μαρ 2018
 *      Author: klapeto
 */

#include "Benchmarks/Image/PngDecoding/PngDecoding.hpp"

#include <Elpida/Engine/Data/RawTaskData.hpp>
#include <Elpida/Config.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/ElpidaException.hpp>

#include "png.h"

namespace Elpida
{

	PngDecoding::PngDecoding(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: ImageTaskBase(specification, processorToRun, serviceProvider, iterationsToRun),
		_outputData(nullptr),
		_width(0),
		_height(0)
	{

	}

	void PngDecoding::run()
	{
		auto input = getInput().getTaskData().get();

		png_image img;
		img.opaque = nullptr;
		img.version = PNG_IMAGE_VERSION;

		if (png_image_begin_read_from_memory(&img, input->getData(), input->getSize()))
		{
			img.format = PNG_FORMAT_RGBA;

			auto newSize = PNG_IMAGE_SIZE(img);
			_outputData = std::make_unique<RawTaskData>(newSize, _processorToRun);

			png_image_finish_read(&img, nullptr, _outputData->getData(), 0, nullptr);

			_width = img.width;
			_height = img.height;
			return;
		}
		else
		{
			std::string error(img.message);
			png_image_free(&img);
			throw ElpidaException(FUNCTION_NAME, Vu::Cs("Failed to encode image: ", error));
		}
	}

	std::optional<TaskDataDto> PngDecoding::finalizeAndGetOutputData()
	{
		return TaskDataDto(std::move(_outputData), {
			{ "width", _width },
			{ "height", _height },
			{ "stride", _width * 4 }
		});
	}

	double PngDecoding::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _width * _height;
	}

} /* namespace Elpida */
