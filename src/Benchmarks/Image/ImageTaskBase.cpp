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

#include "Benchmarks/Image/ImageTaskBase.hpp"

#include <Elpida/Config.hpp>
#include "Elpida/Common/ElpidaException.hpp"
#include <stdexcept>

namespace Elpida
{

	ImageTaskBase::InputImageProperties ImageTaskBase::getImageProperties(const TaskDataDto& taskData)
	{
		auto properties = taskData.getDefinedProperties();
		try
		{
			auto width = properties.at("width");
			auto height = properties.at("height");
			return { (size_t)width, (size_t)height };
		}
		catch (const std::out_of_range& ex)
		{
			throw ElpidaException(FUNCTION_NAME, "Required properties were not defined!");
		}
	}

	ImageTaskBase::ImageTaskBase(const TaskSpecification& specification,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun)
		: WorkloadTask(specification, processorToRun, serviceProvider, iterationsToRun)
	{

	}
}