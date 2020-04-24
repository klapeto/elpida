//
// Created by klapeto on 20/4/20.
//

#include "Elpida/Engine/Configuration/BenchmarkConfiguration.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/ElpidaException.hpp"

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

	void BenchmarkConfiguration::addConfiguration(const TaskSpecification& taskSpecification,
		TaskConfiguration&& configuration)
	{
		auto itr = _taskConfigurations.find(taskSpecification.getId());
		if (itr == _taskConfigurations.end())
		{
			_orderedConfigurations.push_back(std::move(configuration));
			_taskConfigurations.insert_or_assign(taskSpecification.getId(), &_orderedConfigurations.back());
		}
		else
		{
			throw ElpidaException("BenchmarkConfiguration::addConfiguration",
				"Attempted to add a configuration that already exists");
		}
	}

	TaskConfiguration* BenchmarkConfiguration::getConfigurationForTask(const TaskSpecification& taskSpecification)
	{
		return const_cast<TaskConfiguration*>(getConfigurationImpl(taskSpecification));
	}
}