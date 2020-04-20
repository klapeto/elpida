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

	class TaskConfiguration final
	{
	public:
		ConfigurationValueBase* getConfiguration(const std::string& name) const;
		void setConfiguration(const std::string& name, ConfigurationValueBase& configuration);

		TaskConfiguration() = default;
		TaskConfiguration(std::initializer_list<std::pair<std::string, ConfigurationValueBase*>> pairs);;
		TaskConfiguration(TaskConfiguration&&) = default;
		~TaskConfiguration();
	private:
		std::unordered_map<std::string, ConfigurationValueBase*> _configuration;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP
