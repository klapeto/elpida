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

#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverageSpecification.hpp"

#include "Benchmarks/Image/ImageDataPropertiesTransformer.hpp"
#include "Benchmarks/Image/GrayscaleAverage/GrayscaleAverage.hpp"

namespace Elpida
{

	Task* GrayscaleAverageSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun) const
	{
		return new GrayscaleAverage(*this, processorToRun);
	}

	GrayscaleAverageSpecification::GrayscaleAverageSpecification()
		: TaskSpecification("Convert to Gray Scale (Average)",
		ResultSpecification("Conversion Rate",
			"Pixels",
			ResultType::Throughput,
			ResultSpecification::Accumulative,
			"Pixel conversion rate"))
	{
		withDescription("Converts image data to Gray scale using the average of pixels");

		withInputData(DataSpecification("Input Image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The image to convert to grayscale in float pixels"));
		withOutputData(DataSpecification("Output Image data",
			"Pixels",
			4,
			{ "width", "height" },
			"The converted image in float pixels"));

		withDataPropertiesTransformer(new ImageDataPropertiesTransformer());
	}
}