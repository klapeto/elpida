//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Utilities/Uuid.hpp"
#include <utility>

namespace Elpida
{
	TaskSpecification::TaskSpecification(std::string name, ResultSpecification resultSpecification)
		: _name(std::move(name)),
		  _resultSpecification(std::move(resultSpecification)),
		  _acceptsInput(false),
		  _exportsOutput(false),
		  _shouldBeCountedOnResults(false),
		  _multiThreadingEnabled(false),
		  _canBeDisabled(false)
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