//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP
#define INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP

#include <string>
#include <unordered_map>

namespace Elpida
{
	class ConfigurationValueBase;

	/**
	 * Configuration values for a specific Task instance
	 */
	class TaskConfiguration final
	{
	public:
		ConfigurationValueBase* getConfiguration(const std::string& name) const;
		void setConfiguration(const std::string& name, ConfigurationValueBase& configuration);

		const std::unordered_map<std::string, ConfigurationValueBase*>& getAllConfigurations() const
		{
			return _configuration;
		}

		TaskConfiguration() = default;
		TaskConfiguration(std::initializer_list<std::pair<std::string, ConfigurationValueBase*>> pairs);;
		TaskConfiguration(TaskConfiguration&&) = default;
		TaskConfiguration& operator=(TaskConfiguration&&) = default;
		~TaskConfiguration();
	private:
		std::unordered_map<std::string, ConfigurationValueBase*> _configuration;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP
