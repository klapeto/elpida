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

#include "Benchmarks/Image/ConvertToFloat/ConvertToFloatSpecification.hpp"
#include "Benchmarks/Image/ConvertToFloat/ConvertToFloat.hpp"
#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"

namespace Elpida
{

	std::unique_ptr<Task> ConvertToFloatSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		return std::make_unique<ConvertToFloat>(*this, processorToRun, serviceProvider, iterationsToRun);
	}

	ConvertToFloatSpecification::ConvertToFloatSpecification()
		: TaskSpecification("Convert To Float",
		ResultSpecification("Convert Rate",
			"Pixels",
			ResultType::Throughput,
			ResultSpecification::AggregationType::Accumulative),
		"0E442DBF-5E96-4190-81B8-1435765546E0")
	{

		withDescription("Converts an Image with integer values to float values between 0.0 and 1.0");

		withInputData(DataSpecification("Input image",
			"Pixels",
			"stride",
			{ "width", "height", "stride" },
			"Input image in RGBA and integer values (0-255)"));

		withOutputData(DataSpecification("Output image",
			"Pixels",
			"stride",
			{ "width", "height", "stride" },
			"Output image in RGBA and float values (0.0-1.0)"));

		withDataPropertiesTransformer(std::make_shared<ImageDataPropertiesTransformer>());
	}
}