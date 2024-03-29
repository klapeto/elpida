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

#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8Specification.hpp"

#include "Benchmarks/Image/ConvertToUInt8/ConvertToUInt8.hpp"
#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"

namespace Elpida
{

	std::unique_ptr<Task> ConvertToUInt8Specification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		return std::make_unique<ConvertToUInt8>(*this, processorToRun, serviceProvider, iterationsToRun);
	}

	ConvertToUInt8Specification::ConvertToUInt8Specification()
		: TaskSpecification("Convert to UInt8",
		ResultSpecification("Conversion Rate",
			"Pixels",
			ResultType::Throughput,
			ResultSpecification::Accumulative,
			"The rate the pixels are converted from float to int"),
		"050B949C-D4A7-4AC2-8F86-7E659EA2FAB3")
	{

		withDescription("Converts the image data from float to integer");
		withInputData(DataSpecification("Input image data",
			"Pixels",
			"stride",
			{ "width", "height", "stride" },
			"The input image data to convert to integer"));
		withOutputData(DataSpecification("Output image data",
			"Pixels",
			"stride",
			{ "width", "height", "stride" },
			"The output image data that were converted to integer"));

		withDataPropertiesTransformer(std::make_shared<ImageDataPropertiesTransformer>());
	}
}