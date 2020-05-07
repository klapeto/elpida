//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Utilities/Uuid.hpp"
#include <utility>

namespace Elpida
{
	TaskSpecification::TaskSpecification(std::string name,
		std::string description,
		std::string inputValueDescription,
		std::string inputValueUnit,
		std::string outputValueDescription,
		std::string outputValueUnit,
		std::string throughputUnit,
		std::vector<ConfigurationSpecificationBase*>&& configurationSpecifications,
		bool acceptsInput,
		bool exportsOutput,
		bool shouldBeCountedOnResults,
		bool enableMultiThreading,
		bool canBeDisabled,
		std::unordered_map<std::string, ConfigurationValueBase*>&& fixedConfiguration)
		: _name(std::move(name)), _description(std::move(description)),
		  _inputValueDescription(std::move(inputValueDescription)),
		  _inputValueUnit(std::move(inputValueUnit)), _outputValueDescription(std::move(outputValueDescription)),
		  _outputValueUnit(std::move(outputValueUnit)), _throughputUnit(std::move(throughputUnit)),
		  _configurationSpecifications(std::move(configurationSpecifications)),
		  _fixedConfiguration(std::move(fixedConfiguration)),
		  _acceptsInput(acceptsInput),
		  _exportsOutput(exportsOutput),
		  _shouldBeCountedOnResults(shouldBeCountedOnResults),
		  _multiThreadingEnabled(enableMultiThreading),
		  _canBeDisabled(canBeDisabled)
	{
		_id = Uuid::create();
	}

	TaskSpecification::~TaskSpecification()
	{
		for (auto configSpec: _configurationSpecifications)
		{
			delete configSpec;
		}

		for (auto& fixedConfig: _fixedConfiguration)
		{
			delete fixedConfig.second;
		}
	}

	Task* TaskSpecification::createNewTask(const TaskConfiguration& configuration, const TaskAffinity& affinity) const
	{
		TaskConfiguration finalConfig(*this, _fixedConfiguration);
		for (auto& config: configuration.getAllConfigurations())
		{
			auto itr = _fixedConfiguration.find(config.first);
			if (itr == _fixedConfiguration.end())
			{
				finalConfig.setConfiguration(config.first, *config.second);
			}
		}
		return createNewTaskImpl(finalConfig, affinity);
	}
}