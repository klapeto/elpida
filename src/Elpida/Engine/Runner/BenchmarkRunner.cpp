/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Runner/BenchmarkRunner.hpp"

#include "Elpida/Timer.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Calculators/BenchmarkScoreCalculator.hpp"
#include "Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp"
#include "Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp"
#include "Elpida/Engine/Task/TaskBuilder.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Benchmark/Benchmark.hpp"
#include "Elpida/Utilities/RawData.hpp"

namespace Elpida
{
	BenchmarkRunner::BenchmarkRunner()
		: _mustStop(false)
	{

	}

	static void cleanTask(Task* task)
	{
		if (task)
		{
			auto input = task->getInput().getTaskData();
			auto output = task->getOutput().getTaskData();
			delete input;

			if (input != output)	// if the task hasn't propagated the input
			{
				delete output;
			}
			task->resetInputOutput();
		}
	}

	static void assignInput(Task* previousTask, Task* nextTask, bool moreIterationsPending)
	{
		cleanTask(nextTask);	// Destroy input/output of next task

		DataAdapter::adaptAndForwardTaskData(previousTask, nextTask);

		if (!moreIterationsPending)
		{
			cleanTask(previousTask); // On last iteration we destroy the previous task data
		}
	}

	std::vector<BenchmarkResult> BenchmarkRunner::runBenchmarks(const std::vector<BenchmarkRunRequest>& benchmarkRequests,
		const TaskAffinity& taskAffinity)
	{
		_mustStop = false;
		std::vector<BenchmarkResult> benchmarkResults;
		Task* lastExecutedTask = nullptr;
		for (const auto& benchmarkRequest : benchmarkRequests)
		{
			if (_mustStop) break;

			const auto& benchmark = benchmarkRequest.getBenchmark();
			raiseBenchmarkStarted(benchmark);

			auto benchmarkTaskInstances = benchmark.createNewTasks(taskAffinity, benchmarkRequest.getConfiguration());
			try
			{
				std::vector<TaskResult> finalTaskResults;

				for (auto& taskInstance : benchmarkTaskInstances)
				{
					if (_mustStop) break;

					auto& currentTask = taskInstance.getTask();

					raiseTaskStarted(currentTask);

					auto iterations = currentTask.getIterationsToRun();

					std::vector<TaskResult> currentTaskResults;

					for (auto i = 0u; i < iterations; ++i)
					{
						if (_mustStop) break;

						bool morePendingIterations = i < (iterations - 1);

						assignInput(lastExecutedTask, &currentTask, morePendingIterations);

						auto result = runTask(currentTask);

						if (taskInstance.getTaskBuilder().isShouldBeCountedOnResults())
						{
							currentTaskResults.push_back(std::move(result));
						}
					}


					if (taskInstance.getTaskBuilder().isShouldBeCountedOnResults())
					{
						auto calculator = taskInstance.getTaskBuilder().getTaskResultCalculator();
						if (calculator)
						{
							finalTaskResults.push_back(calculator->calculateAggregateResult(currentTaskResults));
						}
						else
						{
							finalTaskResults.push_back(currentTaskResults.back());
						}
					}

					cleanTask(lastExecutedTask);  // lastExecutedTask = previous executed task

					lastExecutedTask = &currentTask;	// set as lastExecutedTask the one we just completed
					raiseTasksEnded(currentTask);
				}

				cleanTask(lastExecutedTask);    // lastExecutedTask = last (final) executed Task

				raiseBenchmarkEnded(benchmark);

				BenchmarkResult::Score score = benchmark.getScoreCalculator().calculate(finalTaskResults);
				benchmarkResults.emplace_back(benchmark, std::move(finalTaskResults), taskAffinity, score);
			}
			catch (...)
			{
				cleanTask(lastExecutedTask);    // lastExecutedTask = last executed Task (Not failing one)
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
		try
		{
			task.prepare();

			auto start = Timer::now();
			task.execute();
			auto end = Timer::now();

			auto result = task.calculateTaskResult(end - start - (Timer::getNowOverhead<>() * 2));

			task.finalize();

			return result;
		}
		catch (...)
		{
			task.finalize();    // Finalize the task to clean it's internal resources
			cleanTask(&task);   // Destroy Input/output
			throw;
		}
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