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

#include "Benchmarks/Image/FloydSteinberg/FloydSteinbergSpecification.hpp"

#include "Benchmarks/Image/FloydSteinberg/FloydSteinberg.hpp"

namespace Elpida
{

	std::unique_ptr<Task> FloydSteinbergSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		size_t iterationsToRun) const
	{
		auto threshold = getSettingAndValidate<ConfigurationType::Float>(configuration,
			Settings::Threshold,
			ConfigurationType::Type::Float).getValue();
		return std::make_unique<FloydSteinberg>(*this, processorToRun, threshold, iterationsToRun);
	}

	FloydSteinbergSpecification::FloydSteinbergSpecification()
		: TaskSpecification("Floyd-Steinberg Dithering",
		ResultSpecification("Conversion Rate",
			"Pixels",
			ResultType::Throughput,
			ResultSpecification::Accumulative))
	{

		withDescription("Converts an Gray Scale image to Black-White dithered with Floyd Steinbergs algorithm");

		withInputData(DataSpecification("Input Image Data",
			"Pixels",
			"stride",
			{ "width", "height", "stride" },
			"Input image data in Grayscale and RGBA float pixels"));
		withOutputData(DataSpecification("Output Image Data",
			"Pixels",
			"stride",
			{ "width", "height", "stride" },
			"Output image data in Black-White dithered and RGBA float pixels"));

		withConfiguration(std::make_shared<ConfigurationSpecification<ConfigurationType::Float>>(0.5,
			"Threshold",
			"The threshold to use for determining if a pixel should be black or white",
			true));

	}
}