//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Benchmark/Benchmark.hpp"

#include <utility>
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Configuration/BenchmarkConfiguration.hpp"
#include "Elpida/Engine/Configuration/TaskConfigurationSpecifications.hpp"
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Utilities/Uuid.hpp"
#include "Elpida/Engine/BenchmarkScoreCalculator.hpp"

namespace Elpida
{

	Benchmark::Benchmark(std::string name,
		std::vector<TaskSpecification*>&& taskSpecifications,
		BenchmarkScoreCalculator* scoreCalculator)
		: _taskSpecifications(std::move(taskSpecifications)),
		  _name(std::move(name)), _scoreCalculator(scoreCalculator)
	{
		_id = Uuid::create();
	}

	Benchmark::~Benchmark()
	{
		for (auto specification: _taskSpecifications)
		{
			delete specification;
		}
		delete _scoreCalculator;
	}

	std::vector<Task*> Benchmark::createNewTasks(const TaskAffinity& affinity,
		const BenchmarkConfiguration& configuration) const
	{
		if (!affinity.getProcessorNodes().empty())
		{
			std::vector<Task*> tasks;
			try
			{
				for (auto spec : _taskSpecifications)
				{
					auto taskConfiguration = configuration.getConfigurationForTask(*spec);
					if (taskConfiguration == nullptr && !spec->getConfigurationSpecifications().empty())
					{
						throw ElpidaException(FUNCTION_NAME,
							Vu::Cs("Task: '",
								spec->getName(),
								"' requires configuration that was not provided!"));
					}
					if (!spec->canBeDisabled() || taskConfiguration->isEnabled())
					{
						if (spec->isMultiThreadingEnabled())
						{
							tasks.push_back(new MultiThreadTask(*spec, *taskConfiguration, affinity));
						}
						else
						{
							tasks.push_back(spec->createNewTask(*taskConfiguration, affinity));
						}
					}
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
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Benchmark affinity is empty");
		}
	}

	std::vector<TaskConfigurationSpecifications> Benchmark::getConfigurationSpecifications() const
	{
		std::vector<TaskConfigurationSpecifications> specifications;
		for (auto spec: _taskSpecifications)
		{
			specifications.emplace_back(*spec);
		}
		return specifications;
	}
}