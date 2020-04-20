//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Configuration/TaskConfiguration.hpp"
#include "Elpida/Engine/Configuration/ConfigurationValueBase.hpp"

namespace Elpida
{

	TaskConfiguration::TaskConfiguration(std::initializer_list<std::pair<std::string,
																		 ConfigurationValueBase*>> pairs)
	{
		for (auto&& pair : pairs)
		{
			_configuration.insert(pair);
		}
	}

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
}