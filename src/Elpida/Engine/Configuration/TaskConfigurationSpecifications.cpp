//
// Created by klapeto on 20/4/20.
//

#include "Elpida/Engine/Configuration/TaskConfigurationSpecifications.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{

	TaskConfigurationSpecifications::TaskConfigurationSpecifications(const TaskSpecification& taskSpecification)
		: _taskSpecification(taskSpecification)
	{
		for (auto confSpec: taskSpecification.getConfigurationSpecifications())
		{
			_configurationSpecifications.push_back(confSpec);
		}
	}
}