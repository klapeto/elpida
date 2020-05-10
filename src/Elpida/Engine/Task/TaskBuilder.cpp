//
// Created by klapeto on 9/5/20.
//

#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Data/Adapters/CopyingChunkNormalizerAdapter.hpp"
#include "Elpida/Engine/Data/Adapters/CopyingUnifyingAdapter.hpp"

namespace Elpida
{

	TaskBuilder::~TaskBuilder()
	{
		delete _taskSpecification;
		delete _dataAdapter;
	}

	TaskBuilder& TaskBuilder::canBeDisabled(bool canBeDisabled)
	{
		_canBeDisabled = canBeDisabled;
		return *this;
	}

	TaskBuilder& TaskBuilder::canBeMultiThreaded(bool canBeMultiThreaded)
	{
		_canBeMultiThreaded = canBeMultiThreaded;
		if (canBeMultiThreaded)
		{
			setNewDataAdapter(*new CopyingChunkNormalizerAdapter());
		}
		else
		{
			setNewDataAdapter(*new CopyingUnifyingAdapter());
		}
		return *this;
	}

	TaskBuilder& TaskBuilder::shouldBeCountedOnResults(bool shouldBeCountedOnResults)
	{
		_shouldBeCountedOnResults = shouldBeCountedOnResults;
		return *this;
	}

	Task* TaskBuilder::build(const TaskConfiguration& configuration, const TaskAffinity& affinity) const
	{
		return _taskSpecification->createNewTask(configuration, affinity);
	}

	void TaskBuilder::fillConfigurationMap()
	{
		for (auto& config : _taskSpecification->getConfigurationSpecifications())
		{
			_configurationMap.emplace(config->getName(), config);
		}
	}

	TaskBuilder& TaskBuilder::withFixedConfiguration(const std::string& name, ConfigurationValueBase& configuration)
	{
		addPredefinedConfigurationValue(name, configuration, true);
		return *this;
	}

	void TaskBuilder::setNewDataAdapter(DataAdapter& newDataAdapter)
	{
		delete _dataAdapter;
		_dataAdapter = &newDataAdapter;
	}

	TaskBuilder& TaskBuilder::withDefaultConfiguration(const std::string& name, ConfigurationValueBase& configuration)
	{
		addPredefinedConfigurationValue(name, configuration, false);
		return *this;
	}

	void TaskBuilder::addPredefinedConfigurationValue(const std::string& name,
		ConfigurationValueBase& value,
		bool fixed)
	{
		auto itr = _configurationMap.find(name);
		if (itr != _configurationMap.end())
		{
			auto existing = _predefinedConfigurationsValues.find(name);
			if (existing != _predefinedConfigurationsValues.end())
			{
				_predefinedConfigurationsValues.erase(existing);
			}
			_predefinedConfigurationsValues.emplace(name, ConfigurationInstance(*itr->second, &value, fixed));
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME,
				Vu::Cs("Setting '",
					name,
					"' is not valid for this Task Specification: '",
					_taskSpecification->getName(),
					"'"));
		}
	}
	void TaskBuilder::generateDefaultConfiguration()
	{
		for (auto& confSpec : _configurationMap)
		{
			auto itr = _predefinedConfigurationsValues.find(confSpec.first);
			if (itr != _predefinedConfigurationsValues.end())
			{
				_defaultConfiguration.setConfiguration(itr->first, *itr->second.getValue()->clone());
			}
			else
			{
				_defaultConfiguration.setConfiguration(confSpec.first, *confSpec.second->createDefault());
			}
		}
	}
}