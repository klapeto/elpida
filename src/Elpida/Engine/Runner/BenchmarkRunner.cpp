//
// Created by klapeto on 19/4/20.
//

#include <Elpida/Timer.hpp>
#include "Elpida/Engine/Runner/BenchmarkRunner.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Task/TaskData.hpp"
#include "Elpida/Engine/BenchmarkScoreCalculator.hpp"
#include "Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp"
#include "Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp"

namespace Elpida
{
	BenchmarkRunner::BenchmarkRunner()
		: _mustStop(false)
	{

	}

	std::vector<BenchmarkResult> BenchmarkRunner::runBenchmarks(const std::vector<BenchmarkRunRequest>& benchmarkRequests,
		const TaskAffinity& taskAffinity)
	{
		_mustStop = false;
		std::vector<BenchmarkResult> benchmarkResults;
		for (const auto& benchmarkRequest : benchmarkRequests)
		{
			if (_mustStop) break;
			const auto& benchmark = benchmarkRequest.getBenchmark();
			raiseBenchmarkStarted(benchmark);

			auto tasks = benchmark.createNewTasks(taskAffinity, benchmarkRequest.getConfiguration());
			try
			{
				std::vector<TaskResult> taskResults;
				for (auto task : tasks)
				{
					if (_mustStop) break;

					raiseTaskStarted(*task);

					auto metrics = runTask(*task);
					if (task->shouldBeCountedOnResults())
					{
						taskResults.emplace_back(task->getSpecification(), std::move(metrics));
					}
					raiseTasksEnded(*task);
				}
				destroyTasks(tasks);
				raiseBenchmarkEnded(benchmark);

				BenchmarkResult::Score score = benchmark.getScoreCalculator().calculate(taskResults);
				benchmarkResults.emplace_back(benchmark, std::move(taskResults), score);
			}
			catch (...)
			{
				destroyTasks(tasks);
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

	TaskMetrics BenchmarkRunner::runTask(Task& task)
	{
		task.applyAffinity();
		task.prepare();

		auto start = Timer::now();
		task.run();
		auto end = Timer::now();

		auto metrics = TaskMetrics(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start),
			task.getInput() != nullptr ? task.getInput()->getSize() : 0,
			(*task.getOutput())->getSize());;

		task.finalize();

		return metrics;
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