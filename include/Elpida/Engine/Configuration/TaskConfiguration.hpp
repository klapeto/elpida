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
	class TaskSpecification;

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

		bool isEnabled() const
		{
			return _enabled;
		}

		void setEnabled(bool enabled)
		{
			_enabled = enabled;
		}

		const TaskSpecification& getTaskSpecification() const
		{
			return *_taskSpecification;
		}

		explicit TaskConfiguration(const TaskSpecification& taskSpecification)
			: _taskSpecification(&taskSpecification), _enabled(true)
		{

		};
		TaskConfiguration(TaskConfiguration&&) = default;
		TaskConfiguration& operator=(TaskConfiguration&&) = default;
		~TaskConfiguration();
	private:
		std::unordered_map<std::string, ConfigurationValueBase*> _configuration;
		const TaskSpecification* _taskSpecification;
		bool _enabled;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_CONFIGURATION_TASKCONFIGURATION_HPP
