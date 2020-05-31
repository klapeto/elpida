/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

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