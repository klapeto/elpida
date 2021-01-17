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

#include <cstring>
#include "Elpida/Engine/Runner/BenchmarkRunner.hpp"

#include "Elpida/Timer.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Calculators/BenchmarkScoreCalculator.hpp"
#include "Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp"
#include "Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp"
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Benchmark/Benchmark.hpp"

namespace Elpida
{
	BenchmarkRunner::BenchmarkRunner()
		: _mustStop(false)
	{

	}

	static TaskDataDto getTaskDataCopy(const TaskDataDto& original)
	{
		if (!original.hasData()) return TaskDataDto(std::unique_ptr<RawTaskData>(), original.getDefinedProperties());

		auto ptr = std::make_unique<RawTaskData>(original.getTaskData()->getSize(),
			original.getTaskData()->getProcessorNode());

		memcpy(ptr->getData(), original.getTaskData()->getData(), ptr->getSize());

		return TaskDataDto(std::move(ptr), original.getDefinedProperties());
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

			auto benchmarkTaskInstances = benchmark.createNewTasks(taskAffinity, benchmarkRequest.getConfiguration());
			try
			{
				std::vector<TaskResult> finalTaskResults;

				TaskDataDto taskData;

				for (auto& taskInstance : benchmarkTaskInstances)
				{
					if (_mustStop) break;

					auto& currentTask = taskInstance.getTask();

					raiseTaskStarted(currentTask);

					auto iterations = currentTask.getIterationsToRun();

					std::vector<TaskResult> currentTaskResults;

					TaskDataDto iterationTaskData;
					for (auto i = 0u; i < iterations; ++i)
					{
						if (_mustStop) break;

						iterationTaskData = getTaskDataCopy(taskData);

						currentTask.setTaskData(iterationTaskData);

						auto result = runTask(currentTask);

						if (taskInstance.getTaskBuilder().shouldBeCountedOnResults())
						{
							currentTaskResults.push_back(result);
						}
					}
					taskData = std::move(iterationTaskData);

					if (taskInstance.getTaskBuilder().shouldBeCountedOnResults())
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

					raiseTasksEnded(currentTask);
				}

				raiseBenchmarkEnded(benchmark);

				BenchmarkResult::Score score = benchmark.getScoreCalculator().calculate(finalTaskResults);
				benchmarkResults.emplace_back(benchmark, std::move(finalTaskResults), taskAffinity, score);
			}
			catch (...)
			{
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