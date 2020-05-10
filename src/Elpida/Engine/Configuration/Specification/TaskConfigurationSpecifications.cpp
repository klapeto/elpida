//
// Created by klapeto on 20/4/20.
//

#include "Elpida/Engine/Configuration/Specification/TaskConfigurationSpecifications.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"

namespace Elpida
{

	TaskConfigurationSpecifications::TaskConfigurationSpecifications(const TaskSpecification& taskSpecification, std::vector<ConfigurationInstance>&& configurations)
		: _taskSpecification(taskSpecification), _configurationInstances(std::move(configurations))
	{
	}
}