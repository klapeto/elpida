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

//
// Created by klapeto on 31/5/20.
//

#include "Benchmarks/Image/PngEncoding/PngEncodingSpecification.hpp"

#include "Benchmarks/Image/PngEncoding/PngEncoding.hpp"

namespace Elpida
{
	std::unique_ptr<Task> PngEncodingSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		return std::make_unique<PngEncoding>(*this, processorToRun, serviceProvider, iterationsToRun);
	}

	PngEncodingSpecification::PngEncodingSpecification()
		: TaskSpecification("Encode to PNG",
		ResultSpecification("Encode rate",
			"Pixels",
			ResultType::Throughput,
			ResultSpecification::Accumulative,
			"The rate that pixels are encoded into PNG format"),
		"30948762-9B7B-4CEA-8D9B-6913F3DA2097")
	{

		withDescription("Encodes the image data to PNG Format");
		withInputData(DataSpecification("Input image data",
			"Pixels",
			"stride",
			{ "width", "height", "stride"  },
			"The input image data in UInt8 to encode into PNG"));
		withOutputData(DataSpecification("PNG data", "B", "The image encoded to PNG format"));
	}
}
