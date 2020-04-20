//
// Created by klapeto on 20/4/20.
//

#include "Elpida/Engine/Configuration/BenchmarkConfiguration.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{

	TaskConfiguration* BenchmarkConfiguration::getConfigurationForTask(const std::string& taskKey) const
	{
		auto itr = _taskConfigurations.find(taskKey);
		if (itr != _taskConfigurations.end())
		{
			return itr->second;
		}
		return nullptr;
	}
}