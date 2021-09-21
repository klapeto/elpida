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
#include "Elpida/Engine/Data/DataAdapter.hpp"
#include "Elpida/Engine/Benchmark/Benchmark.hpp"
#include "Elpida/ServiceProvider.hpp"
#include "Elpida/SystemInfo/TimingInfo.hpp"
#include "Elpida/Utilities/Statistics.hpp"

#include "Elpida/Engine/Task/ExecutionParameters.hpp"
#include "Elpida/Engine/Runner/MicroTaskRunner.hpp"
#include "Elpida/Engine/Runner/DefaultTaskRunner.hpp"

#include <cstring>
#include <thread>

namespace Elpida
{
	BenchmarkRunner::BenchmarkRunner(const ServiceProvider& serviceProvider)
			: _mustStop(false), _serviceProvider(serviceProvider)
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

	std::vector<BenchmarkResult>
	BenchmarkRunner::runBenchmarks(const std::vector<BenchmarkRunRequest>& benchmarkRequests,
			const TaskAffinity& taskAffinity)
	{
		_mustStop = false;
		std::vector<BenchmarkResult> benchmarkResults;
		for (const auto& benchmarkRequest : benchmarkRequests)
		{
			if (_mustStop) break;

			const auto& benchmark = benchmarkRequest.getBenchmark();
			raiseBenchmarkStarted(benchmark);

			auto benchmarkTaskInstances =
					benchmark.createNewTasks(taskAffinity, benchmarkRequest.getConfiguration(), _serviceProvider);
			try
			{
				std::vector<ProcessedTaskResult> finalTaskResults;

				TaskDataDto taskData;

				for (auto& taskInstance : benchmarkTaskInstances)
				{
					if (_mustStop) break;

					auto& currentTask = taskInstance.getTask();
					auto& taskBuilder = taskInstance.getTaskBuilder();

					raiseTaskStarted(taskBuilder);

					auto iterations = currentTask.getIterationsToRun();

					std::vector<TaskResult> currentTaskResults;

					TaskDataDto iterationTaskData;
					for (auto i = 0u; i < iterations; ++i)
					{
						if (_mustStop) break;

						raiseTaskIterationStarted(taskBuilder, i);

						iterationTaskData = getTaskDataCopy(taskData);

						currentTask.setTaskData(iterationTaskData);

						auto result = runTask(currentTask);

						if (taskBuilder.shouldBeCountedOnResults())
						{
							currentTaskResults.push_back(result);
						}

						raiseTaskIterationEnded(taskBuilder, i);
					}
					taskData = std::move(iterationTaskData);

					if (taskBuilder.shouldBeCountedOnResults())
					{
						auto calculator = taskBuilder.getTaskResultCalculator();
						if (calculator)
						{
							finalTaskResults.push_back(calculator->calculateAggregateResult(currentTaskResults));
						}
						else
						{
							auto statistics = Statistics::calculateBasicStatistics(currentTaskResults,
									[](const TaskResult& x)
									{
										return x.getMetrics().getDuration().count();
									});
							ProcessedTaskResult processedTaskResult(currentTaskResults.back().getMetrics(),
									statistics,
									std::vector<TaskMetrics>(),
									taskBuilder.getTaskSpecification());
							finalTaskResults.push_back(std::move(processedTaskResult));
						}
					}

					raiseTasksEnded(taskBuilder);
				}

				raiseBenchmarkEnded(benchmark);

				auto score = benchmark.getScoreCalculator().calculate(benchmark, finalTaskResults);
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
		TaskResult result(task.getSpecification(), TaskMetrics(Duration::zero(), 0, 0));

		std::string exceptionMessage;

		auto thread = std::thread([&task, &result, &exceptionMessage]
		{
			try
			{
				ExecutionParameters parameters;
				result = task.execute(parameters);
			}
			catch (const std::exception& ex)
			{
				exceptionMessage = ex.what();
			}
		});

		thread.join();

		if (!exceptionMessage.empty())
		{
			throw ElpidaException(exceptionMessage);
		}

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

	void BenchmarkRunner::raiseTaskStarted(const TaskBuilder& taskBuilder)
	{
		TaskEventArgs args(taskBuilder);
		taskStarted.raise(args);
	}

	void BenchmarkRunner::raiseTasksEnded(const TaskBuilder& taskBuilder)
	{
		TaskEventArgs args(taskBuilder);
		taskEnded.raise(args);
	}

	void BenchmarkRunner::raiseTaskIterationStarted(const TaskBuilder& taskBuilder, size_t iteration)
	{
		TaskEventArgs args(taskBuilder, iteration);
		taskIterationStarted.raise(args);
	}

	void BenchmarkRunner::raiseTaskIterationEnded(const TaskBuilder& taskBuilder, size_t iteration)
	{
		TaskEventArgs args(taskBuilder, iteration);
		taskIterationEnded.raise(args);
	}

}