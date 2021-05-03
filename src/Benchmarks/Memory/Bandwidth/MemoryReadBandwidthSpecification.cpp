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
// Created by klapeto on 28/4/20.
//

#include "Benchmarks/Memory/Bandwidth/MemoryReadBandwidthSpecification.hpp"

#include "Benchmarks/Memory/Bandwidth/MemoryReadBandwidth.hpp"
#include "Benchmarks/Memory/WorkingSetSizes.hpp"

namespace Elpida
{

	MemoryReadBandwidthSpecification::MemoryReadBandwidthSpecification()
		: TaskSpecification("Memory Read Bandwidth",
		ResultSpecification("Read Rate", "B", ResultType::Throughput, ResultSpecification::Accumulative),
		"65d80c67-13e4-44c8-b039-c846ae160fe3")
	{
		withDescription("Reads continuously a memory region to determine Memory Read Bandwidth");

		withConfiguration(std::make_shared<ConfigurationSpecification<ConfigurationType::Function<size_t(const ProcessorNode&)>>>(
			ConfigurationType::Type::Function,
			ConfigurationType::Function<size_t(const ProcessorNode&)>(),
			Settings::MemorySizeCalculator,
			"The function to calculate the amount of memory to test",
			false));
	}

	std::unique_ptr<Task> MemoryReadBandwidthSpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{

		auto& func = getSettingAndValidate<ConfigurationType::Function<size_t(const ProcessorNode&)>>(configuration,
			std::string(Settings::MemorySizeCalculator),
			ConfigurationType::Type::Function);

		return std::make_unique<MemoryReadBandwidth>(*this,
			processorToRun,
			serviceProvider,
			func.getValue()(processorToRun),
			iterationsToRun);
	}
}