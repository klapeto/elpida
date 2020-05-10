//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/Concrete/ConfigurationValueBase.hpp"

namespace Elpida
{

	TaskConfiguration::~TaskConfiguration()
	{
		for (auto& pair: _configuration)
		{
			delete pair.second;
		}
	}

	void TaskConfiguration::setConfiguration(const std::string& name, ConfigurationValueBase& configuration)
	{
		auto itr = _configuration.find(name);
		if (itr != _configuration.end())
		{
			delete itr->second;
			_configuration.erase(itr);
		}
		_configuration.emplace(name, &configuration);
	}

	ConfigurationValueBase* TaskConfiguration::getConfiguration(const std::string& name) const
	{
		auto itr = _configuration.find(name);
		if (itr != _configuration.end())
		{
			return itr->second;
		}
		return nullptr;
	}

	TaskConfiguration::TaskConfiguration(const TaskConfiguration& other)
	{
		copy(other);
	}

	TaskConfiguration& TaskConfiguration::operator=(const TaskConfiguration& other)
	{
		copy(other);
		return *this;
	}

	void TaskConfiguration::copy(const TaskConfiguration& other)
	{
		for (auto& conf: other._configuration)
		{
			_configuration.emplace(conf.first, conf.second->clone());
		}
		_taskSpecification = other._taskSpecification;
		_enabled = other._enabled;
		_taskBuilder = other._taskBuilder;
	}
}