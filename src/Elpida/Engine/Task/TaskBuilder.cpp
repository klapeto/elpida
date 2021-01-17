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
// Created by klapeto on 9/5/20.
//

#include "Elpida/Engine/Task/TaskBuilder.hpp"

#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Task/Task.hpp"

namespace Elpida
{

	TaskBuilder& TaskBuilder::canBeDisabled(bool canBeDisabled)
	{
		_canBeDisabled = canBeDisabled;
		return *this;
	}

	TaskBuilder& TaskBuilder::canBeMultiThreaded(bool canBeMultiThreaded)
	{
		_canBeMultiThreaded = canBeMultiThreaded;
		return *this;
	}

	TaskBuilder& TaskBuilder::shouldBeCountedOnResults(bool shouldBeCountedOnResults)
	{
		_shouldBeCountedOnResults = shouldBeCountedOnResults;
		return *this;
	}

	std::unique_ptr<Task> TaskBuilder::build(const TaskConfiguration& configuration, const ProcessorNode& processorToRun) const
	{
		return _taskSpecification->createNewTask(configuration, processorToRun, _iterationsToRun);
	}

	void TaskBuilder::fillConfigurationMap()
	{
		for (auto& config : _taskSpecification->getConfigurationSpecifications())
		{
			_configurationMap.emplace(config->getName(), config);
		}
	}

	TaskConfiguration TaskBuilder::generateDefaultConfiguration() const
	{
		TaskConfiguration config(*_taskSpecification, *this);

		for (auto& confSpec : _configurationMap)
		{
			auto itr = _predefinedConfigurationsValues.find(confSpec.first);
			if (itr != _predefinedConfigurationsValues.end())
			{
				config.setConfiguration(itr->first, itr->second.getValue().clone());
			}
			else
			{
				config.setConfiguration(confSpec.first, confSpec.second->createDefault());
			}
		}
		return config;
	}
}