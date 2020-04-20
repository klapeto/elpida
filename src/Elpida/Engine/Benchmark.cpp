//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Benchmark.hpp"

#include <utility>
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Configuration/BenchmarkConfiguration.hpp"


namespace Elpida
{

	Benchmark::Benchmark(std::string name,
		std::vector<TaskSpecification*>&& taskSpecifications,
		const BenchmarkScoreCalculator& scoreCalculator)
		: _taskSpecifications(std::move(taskSpecifications)),
		  _name(std::move(name)), _scoreCalculator(scoreCalculator)
	{
	}

	Benchmark::~Benchmark()
	{
		for (auto specification: _taskSpecifications)
		{
			delete specification;
		}
	}

	std::vector<Task*> Benchmark::createNewTasks(const TaskAffinity& affinity,
		const BenchmarkConfiguration& configuration)
	{
		std::vector<Task*> tasks;
		Task* previousTask = nullptr;
		auto i = 1u;
		try
		{
			for (auto spec : _taskSpecifications)
			{
				auto taskConfiguration = configuration.getConfigurationForTask(getKeyForTask(*spec, i++));
				if (taskConfiguration == nullptr && !spec->getConfigurationSpecifications().empty())
				{
					throw ElpidaException("Benchmark::createNewTasks",
						Vu::concatenateToString("Task: '",
							spec->getName(),
							"' requires configuration that was not provided!"));
				}
				auto task = spec->createNewTask(*taskConfiguration, affinity);
				if (previousTask != nullptr)
				{
					task->setInput(previousTask->getOutput());
				}
				previousTask = task;
				tasks.push_back(task);
			}
		}
		catch (...)
		{
			for (auto task : tasks)
			{
				delete task;
			}
			throw;
		}
		return tasks;
	}

	ConfigurationSpecificationGroups Benchmark::getConfigurationSpecifications() const
	{
		std::unordered_map<std::string, std::vector<ConfigurationSpecification*>> groups;
		auto i = 1u;
		for (auto spec: _taskSpecifications)
		{
			std::vector<ConfigurationSpecification*> configSpecs;
			for (auto configSpec : spec->getConfigurationSpecifications())
			{
				configSpecs.push_back(&configSpec);
			}
			groups.emplace(getKeyForTask(*spec, i++), std::move(configSpecs));
		}
		return ConfigurationSpecificationGroups(std::move(groups));
	}

	std::string Benchmark::getKeyForTask(const TaskSpecification& specification, size_t index)
	{
		return specification.getName() + " #" + std::to_string(index);
	}
}