//
// Created by klapeto on 19/4/20.
//


#include "Elpida/Engine/Runner/BenchmarkRunner.hpp"

#include <cmath>
#include <cstring>


#include "Elpida/Timer.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Task/TaskDataDto.hpp"
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
#include "Elpida/Engine/Data/DataAdapter.hpp"

namespace Elpida
{
	BenchmarkRunner::BenchmarkRunner()
		: _mustStop(false)
	{

	}

	static void assignInput(Task* previousTask, Task* nextTask)
	{
		DataAdapter::adaptAndForwardTaskData(previousTask, nextTask);
	}

	std::vector<BenchmarkResult> BenchmarkRunner::runBenchmarks(const std::vector<BenchmarkRunRequest>& benchmarkRequests,
		const TaskAffinity& taskAffinity)
	{
		_mustStop = false;
		std::vector<BenchmarkResult> benchmarkResults;
		Task* previousTask = nullptr;
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

					assignInput(previousTask, &task);

					previousTask = &task;
					auto result = runTask(task);

					if (taskInstance.getTaskBuilder().isShouldBeCountedOnResults())
					{
						taskResults.push_back(std::move(result));
					}
					raiseTasksEnded(task);
				}
				assignInput(previousTask, nullptr);
				raiseBenchmarkEnded(benchmark);

				BenchmarkResult::Score score = benchmark.getScoreCalculator().calculate(taskResults);
				benchmarkResults.emplace_back(benchmark, std::move(taskResults), score);
			}
			catch (...)
			{
				assignInput(previousTask, nullptr);
				throw;
			}
		}


		return benchmarkResults;
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

		auto result = task.calculateTaskResult(end - start);

		task.finalize();

		return result;
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