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
// Created by klapeto on 20/4/20.
//

#include "Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp"

#include "Elpida/Engine/Benchmark/Benchmark.hpp"

namespace Elpida
{

	std::optional<std::reference_wrapper<const TaskConfiguration>> BenchmarkConfiguration::getConfigurationForTask(const TaskBuilder& taskBuilder) const
	{
		auto ptr = getConfigurationImpl(taskBuilder);
		if (ptr)
		{
			return std::cref(*ptr);
		}
		return std::nullopt;
	}

	TaskConfiguration* BenchmarkConfiguration::getConfigurationImpl(const TaskBuilder& taskBuilder) const
	{
		auto itr = _taskConfigurations.find(taskBuilder.getId());
		if (itr != _taskConfigurations.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	std::optional<std::reference_wrapper<TaskConfiguration>> BenchmarkConfiguration::getConfigurationForTask(const TaskBuilder& taskBuilder)
	{
		auto ptr = getConfigurationImpl(taskBuilder);
		if (ptr)
		{
			return *ptr;
		}
		return std::nullopt;
	}

	BenchmarkConfiguration::BenchmarkConfiguration(const Benchmark& benchmark)
	{
		_orderedConfigurations.reserve(benchmark.getTotalTasksCount());
		// The reason we need to reserve in advance is due to the std::unordered_map has pointers to
		// the std::vector objects. If the std::vector reallocates with an addition, then the pointers
		// inside the std::map will be invalidated. With reserve() we prevent this (Assuming that the
		// additions will not exceed the benchmarks task count)
	}
}