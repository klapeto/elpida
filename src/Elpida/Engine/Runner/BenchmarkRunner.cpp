//
// Created by klapeto on 19/4/20.
//


#include "Elpida/Engine/Runner/BenchmarkRunner.hpp"

#include <cmath>
#include <cstring>

#include "Elpida/Timer.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Task/TaskInput.hpp"
#include "Elpida/Engine/Task/TaskOutput.hpp"
#include "Elpida/Engine/BenchmarkScoreCalculator.hpp"
#include "Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp"
#include "Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Data/ActiveTaskData.hpp"
#include "Elpida/Engine/Data/PassiveTaskData.hpp"
#include "Elpida/Utilities/NumaMemory.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"

namespace Elpida
{
	BenchmarkRunner::BenchmarkRunner()
		: _mustStop(false)
	{

	}

	static void assignInput(BenchmarkTaskInstance* previousTaskInstance,
		BenchmarkTaskInstance* nextTaskInstance)
	{
		auto& nextTask = nextTaskInstance->getTask();
		auto& nextSpec = nextTask.getSpecification();
		if (previousTaskInstance != nullptr && nextSpec.acceptsInput())
		{
			auto& previousTask = previousTaskInstance->getTask();
			auto& previousOutput = previousTask.getOutput();
			nextTask.setInput(TaskInput(*new PassiveTaskData(previousOutput.getTaskData())));
		}
	}

	std::vector<BenchmarkResult> BenchmarkRunner::runBenchmarks(const std::vector<BenchmarkRunRequest>& benchmarkRequests,
		const TaskAffinity& taskAffinity)
	{
		_mustStop = false;
		std::vector<BenchmarkResult> benchmarkResults;
		BenchmarkTaskInstance* previousTask = nullptr;
		for (const auto& benchmarkRequest : benchmarkRequests)
		{
			if (_mustStop) break;
			const auto& benchmark = benchmarkRequest.getBenchmark();
			raiseBenchmarkStarted(benchmark);

			auto benchmarkTaskInstances = benchmark.createNewTasks(taskAffinity, benchmarkRequest.getConfiguration());
			try
			{
				std::vector<TaskResult> taskResults;
				for (auto& taskInstance : benchmarkTaskInstances)
				{
					if (_mustStop) break;
					auto& task = taskInstance.getTask();
					raiseTaskStarted(task);

					assignInput(previousTask, &taskInstance);

					auto result = runTask(task);

					previousTask = &taskInstance;
					if (taskInstance.getTaskBuilder().isShouldBeCountedOnResults())
					{
						taskResults.push_back(std::move(result));
					}
					raiseTasksEnded(task);
				}
				//destroyTasks(benchmarkTaskInstances);
				raiseBenchmarkEnded(benchmark);

				BenchmarkResult::Score score = benchmark.getScoreCalculator().calculate(taskResults);
				benchmarkResults.emplace_back(benchmark, std::move(taskResults), score);
			}
			catch (...)
			{
				//destroyTasks(benchmarkTaskInstances);
				throw;
			}
		}

		return benchmarkResults;
	}

	void BenchmarkRunner::destroyTasks(std::vector<Task*>& tasks)
	{
		for (auto task : tasks)
		{
			delete task;
		}
		tasks.clear();
	}

	void BenchmarkRunner::stopBenchmarking()
	{
		_mustStop = true;
	}

	TaskResult BenchmarkRunner::runTask(Task& task)
	{
		task.applyAffinity();
		task.prepare();

		auto start = Timer::now();
		task.execute();
		auto end = Timer::now();

		task.finalize();

		return task.calculateTaskResult(end - start);
	}

	void BenchmarkRunner::raiseBenchmarkStarted(const Benchmark& benchmark)
	{
		BenchmarkEventArgs args(benchmark);
		benchmarkStarted.raise(args);
	}

	void BenchmarkRunner::raiseBenchmarkEnded(const Benchmark& benchmark)
	{
		BenchmarkEventArgs args(benchmark);
		benchmarkEnded.raise(args);
	}

	void BenchmarkRunner::raiseTaskStarted(const Task& task)
	{
		TaskEventArgs args(task.getSpecification());
		taskStarted.raise(args);
	}

	void BenchmarkRunner::raiseTasksEnded(const Task& task)
	{
		TaskEventArgs args(task.getSpecification());
		taskEnded.raise(args);
	}

}