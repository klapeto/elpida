//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Benchmark/Benchmark.hpp"

#include <utility>
#include "Elpida/Config.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp"
#include "Elpida/Engine/Configuration/Specification/TaskConfigurationSpecifications.hpp"
#include "Elpida/Engine/Task/MultiThreadTask.hpp"
#include "Elpida/Utilities/Uuid.hpp"
#include "Elpida/Engine/BenchmarkScoreCalculator.hpp"

namespace Elpida
{

	Benchmark::Benchmark(std::string name,
		std::vector<TaskBuilder*>&& taskSpecifications,
		BenchmarkScoreCalculator* scoreCalculator)
		: _taskBuilders(std::move(taskSpecifications)),
		  _name(std::move(name)), _scoreCalculator(scoreCalculator)
	{
		_id = Uuid::create();
	}

	Benchmark::~Benchmark()
	{
		for (auto builder: _taskBuilders)
		{
			delete builder;
		}
		delete _scoreCalculator;
	}

	std::vector<BenchmarkTaskInstance> Benchmark::createNewTasks(const TaskAffinity& affinity,
		const BenchmarkConfiguration& configuration) const
	{
		if (!affinity.getProcessorNodes().empty())
		{
			std::vector<BenchmarkTaskInstance> tasks;
			try
			{
				for (auto builder : _taskBuilders)
				{
					auto taskConfiguration = configuration.getConfigurationForTask(builder->getTaskSpecification());
					if (taskConfiguration == nullptr && !builder->getDefaultConfiguration().getAllConfigurations().empty())
					{
						throw ElpidaException(FUNCTION_NAME,
							Vu::Cs("Task: '",
								builder->getTaskSpecification().getName(),
								"' requires configuration that was not provided!"));
					}
					if (!builder->isCanBeDisabled() || taskConfiguration->isEnabled())
					{
						if (builder->isCanBeMultiThreaded())
						{
							tasks.emplace_back(*new MultiThreadTask(*builder, *taskConfiguration, affinity), *builder);
						}
						else
						{
							tasks.emplace_back(*builder->build(*taskConfiguration, affinity), *builder);
						}
					}
				}
			}
			catch (...)
			{
				throw;
			}
			return tasks;
		}
		else
		{
			throw ElpidaException(FUNCTION_NAME, "Benchmark affinity is empty");
		}
	}
}