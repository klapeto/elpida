//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP

#include <vector>
#include "Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/ConfigurationInstance.hpp"

namespace Elpida
{
	class TaskSpecification;

	/**
	 * A collection of Configuration Specifications for a TaskSpecification
	 * These are exposed from a Benchmark so that the outside have grouped
	 * configurations
	 */
	class TaskConfigurationSpecifications final
	{
	public:
		[[nodiscard]] const TaskSpecification& getTaskSpecification() const
		{
			return _taskSpecification;
		}

//		[[nodiscard]] const std::vector<ConfigurationInstance>& getConfigurationInstances() const
//		{
//			return _configurationInstances;
//		}

		explicit TaskConfigurationSpecifications(const TaskSpecification& taskSpecification,
			std::vector<ConfigurationInstance>&& configurations);
		~TaskConfigurationSpecifications() = default;
	private:
		const TaskSpecification& _taskSpecification;
		std::vector<ConfigurationInstance> _configurationInstances;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP
