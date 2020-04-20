//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP

#include <vector>

namespace Elpida
{
	class TaskSpecification;
	class ConfigurationSpecification;

	/**
	 * A collection of Configuration Specifications for a TaskSpecification
	 */
	class TaskConfigurationSpecifications final
	{
	public:
		[[nodiscard]] const TaskSpecification& getTaskSpecification() const
		{
			return _taskSpecification;
		}

		[[nodiscard]] const std::vector<const ConfigurationSpecification*>& getConfigurationSpecifications() const
		{
			return _configurationSpecifications;
		}

		explicit TaskConfigurationSpecifications(const TaskSpecification& taskSpecification);
	private:
		const TaskSpecification& _taskSpecification;
		std::vector<const ConfigurationSpecification*> _configurationSpecifications;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP
