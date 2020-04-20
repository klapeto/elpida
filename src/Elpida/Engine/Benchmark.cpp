//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Benchmark.hpp"

#include <utility>
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Configuration/BenchmarkConfiguration.hpp"
#include "Elpida/Engine/Configuration/TaskConfigurationSpecifications.hpp"
#include "Elpida/Engine/Task/MultiThreadTask.hpp"

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
		const BenchmarkConfiguration& configuration) const
	{
		std::vector<Task*> tasks;
		Task* previousTask = nullptr;
		try
		{
			for (auto spec : _taskSpecifications)
			{
				auto taskConfiguration = configuration.getConfigurationForTask(*spec);
				if (taskConfiguration == nullptr && !spec->getConfigurationSpecifications().empty())
				{
					throw ElpidaException(FUNCTION_NAME,
						Vu::concatenateToString("Task: '",
							spec->getName(),
							"' requires configuration that was not provided!"));
				}
				Task* task = nullptr;

				if (spec->isMultiThreadingEnabled())
				{
					task = new MultiThreadTask(*spec, *taskConfiguration, affinity);
				}
				else
				{
					task = spec->createNewTask(*taskConfiguration, affinity);
				}

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

	std::vector<TaskConfigurationSpecifications> Benchmark::getConfigurationSpecifications() const
	{
		std::vector<TaskConfigurationSpecifications> specifications;
		for (auto spec: _taskSpecifications)
		{
			specifications.push_back(TaskConfigurationSpecifications(*spec));
		}
		return specifications;
	}
}