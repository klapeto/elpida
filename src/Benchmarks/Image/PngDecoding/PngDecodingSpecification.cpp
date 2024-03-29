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

#include "Benchmarks/Image/PngDecoding/PngDecodingSpecification.hpp"

#include "Benchmarks/Image/PngDecoding/PngDecoding.hpp"

namespace Elpida
{

	std::unique_ptr<Task> PngDecodingSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		return std::make_unique<PngDecoding>(*this, processorToRun, serviceProvider, iterationsToRun);
	}

	PngDecodingSpecification::PngDecodingSpecification()
		: TaskSpecification("Png Decoding",
		ResultSpecification("Decode Rate", "Pixels", ResultType::Throughput, ResultSpecification::Accumulative),
		"B0B5E44D-04B1-44AE-9C1D-757B0E97A652")
	{
		withDescription("Decodes raw Png data to RGBA pixel array");
		withInputData(DataSpecification("Input file data", "B", "The raw png file data"));
		withOutputData(DataSpecification("Output image data",
			"Pixels",
			"stride",
			{ "width", "height", "stride" },
			"The image data in RGBA pixel array"));
	}
}