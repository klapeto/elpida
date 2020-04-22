//
// Created by klapeto on 20/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP

#include <vector>

namespace Elpida
{
	class TaskSpecification;
	class ConfigurationSpecificationBase;

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

		[[nodiscard]] const std::vector<const ConfigurationSpecificationBase*>& getConfigurationSpecifications() const
		{
			return _configurationSpecifications;
		}

		explicit TaskConfigurationSpecifications(const TaskSpecification& taskSpecification);
		~TaskConfigurationSpecifications() = default;
	private:
		const TaskSpecification& _taskSpecification;
		std::vector<const ConfigurationSpecificationBase*> _configurationSpecifications;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATIONSPECIFICATIONS_HPP
