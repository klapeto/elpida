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
// Created by klapeto on 19/4/20.
//

#include "Elpida/CommonTasks/AllocateMemory/AllocateMemorySpecification.hpp"

#include "Elpida/CommonTasks/AllocateMemory/AllocateMemory.hpp"
#include "Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp"
#include "Elpida/Engine/Data/DataSpecification.hpp"
#include "Elpida/Engine/Configuration/Specification/ConfigurationSpecification.hpp"

namespace Elpida
{

	std::unique_ptr<Task> AllocateMemorySpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		auto& size = getSettingAndValidate<ConfigurationType::UnsignedInt>(configuration,
			std::string(Settings::MemorySize),
			ConfigurationType::Type::UnsignedInt);

		return std::make_unique<AllocateMemory>(*this, processorToRun, serviceProvider, size.getValue(), iterationsToRun);
	}

	AllocateMemorySpecification::AllocateMemorySpecification()
		: TaskSpecification("Allocate Memory",
		ResultSpecification("Allocation Rate", "B", ResultType::Throughput, ResultSpecification::Accumulative),
		"6CB6FD86-A231-43A1-86D5-D16B00636D17")
	{
		withDescription("Allocates memory to be used by other tasks on the benchmark");

		withOutputData(DataSpecification("Allocated Bytes", "B", "The allocated memory"));

		withConfiguration(std::make_shared<ConfigurationSpecification<ConfigurationType::UnsignedInt>>(256ul,
			Settings::MemorySize,
			"The amount of memory to allocate",
			true));
	}
}

