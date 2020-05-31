//
// Created by klapeto on 9/5/20.
//


#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"

namespace Elpida
{

	TaskBuilder::~TaskBuilder()
	{
		delete _taskSpecification;
	}

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

	Task* TaskBuilder::build(const TaskConfiguration& configuration, const ProcessorNode& processorToRun) const
	{
		return _taskSpecification->createNewTask(configuration, processorToRun);
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
				config.setConfiguration(itr->first, *itr->second.getValue()->clone());
			}
			else
			{
				config.setConfiguration(confSpec.first, *confSpec.second->createDefault());
			}
		}
		return config;
	}
}