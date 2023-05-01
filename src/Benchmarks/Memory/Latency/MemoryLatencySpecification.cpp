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
// Created by klapeto on 30/4/20.
//

#include "Benchmarks/Memory/Latency/MemoryLatencySpecification.hpp"

#include "Benchmarks/Memory/Latency/MemoryLatency.hpp"

namespace Elpida
{

	MemoryLatencySpecification::MemoryLatencySpecification(const std::string& name, const std::string& uuid)
		: TaskSpecification(name,
		ResultSpecification("Access time", "s", ResultType::Raw, ResultSpecification::Average),
		uuid)
	{
		withDescription("Reads continuously a memory region to determine memory read latency");

		withConfiguration(std::make_shared<ConfigurationSpecification<ConfigurationType::Function<size_t(const ProcessorNode&)>>>(
			ConfigurationType::Type::Function,
			ConfigurationType::Function<size_t(const ProcessorNode&)>(),
			Settings::MemorySizeCalculator,
			"The function to calculate the amount of memory to test",
			false));
	}

	std::unique_ptr<Task> MemoryLatencySpecification::createNewTask(const TaskConfiguration& configuration,
		const ProcessorNode& processorToRun,
		const ServiceProvider& serviceProvider,
		size_t iterationsToRun) const
	{
		auto& func = getSettingAndValidate<ConfigurationType::Function<size_t(const ProcessorNode&)>>(configuration,
			std::string(Settings::MemorySizeCalculator),
			ConfigurationType::Type::Function);

		return std::make_unique<MemoryLatency>(*this,
			processorToRun,
			serviceProvider,
			func.getValue()(processorToRun),
			iterationsToRun);

	}
}