//
// Created by klapeto on 20/4/20.
//


#include "Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp"
#include "Elpida/Engine/Benchmark/Benchmark.hpp"


namespace Elpida
{

	const TaskConfiguration* BenchmarkConfiguration::getConfigurationForTask(const TaskSpecification& taskSpecification) const
	{
		return getConfigurationImpl(taskSpecification);
	}

	const TaskConfiguration* BenchmarkConfiguration::getConfigurationImpl(const TaskSpecification& taskSpecification) const
	{
		auto itr = _taskConfigurations.find(taskSpecification.getId());
		if (itr != _taskConfigurations.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	TaskConfiguration* BenchmarkConfiguration::getConfigurationForTask(const TaskSpecification& taskSpecification)
	{
		return const_cast<TaskConfiguration*>(getConfigurationImpl(taskSpecification));
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