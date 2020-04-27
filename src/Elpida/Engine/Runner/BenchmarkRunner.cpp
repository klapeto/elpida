//
// Created by klapeto on 19/4/20.
//


#include "Elpida/Engine/Runner/BenchmarkRunner.hpp"

#include <cmath>
#include <cstring>

#include "Elpida/Timer.hpp"
#include "Elpida/Engine/Task/Task.hpp"
#include "Elpida/Engine/Task/Data/TaskData.hpp"
#include "Elpida/Engine/Task/TaskInput.hpp"
#include "Elpida/Engine/Task/TaskOutput.hpp"
#include "Elpida/Engine/BenchmarkScoreCalculator.hpp"
#include "Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp"
#include "Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/Engine/Task/Data/ActiveTaskData.hpp"
#include "Elpida/Engine/Task/Data/PassiveTaskData.hpp"
#include "Elpida/Utilities/NumaMemory.hpp"
#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Task/InputManipulator.hpp"

namespace Elpida
{
	BenchmarkRunner::BenchmarkRunner()
		: _mustStop(false)
	{

	}

	static void handle(Task* previousTask, Task* nextTask, const TaskAffinity& affinity)
	{
		auto& nextSpec = nextTask->getSpecification();
		if (previousTask != nullptr && nextSpec.acceptsInput())
		{
			auto& previousOutput = previousTask->getOutput();
			bool nextManyInputs = nextSpec.isMultiThreadingEnabled();

			if (nextManyInputs)
			{
				// N -> N
				nextTask->setInput(InputManipulator::getChunkedInput(previousOutput, affinity));
			}
			else
			{
				nextTask->setInput(InputManipulator::getUnifiedInput(previousOutput, affinity));
			}
		}
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

			auto tasks = benchmark.createNewTasks(taskAffinity, benchmarkRequest.getConfiguration());
			try
			{
				std::vector<TaskResult> taskResults;
				for (auto task : tasks)
				{
					if (_mustStop) break;

					raiseTaskStarted(*task);


					handle(previousTask, task, taskAffinity);
//					if (previousTask != nullptr)
//					{
//						task->setInput(previousTask->getOutput().createTaskInput());
//					}
					auto metrics = runTask(*task);
					previousTask = task;
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

	template<typename T, typename TCallable>
	static double accumulateOutputValues(const std::vector<T>& vec, TCallable callable)
	{
		double returnValue = 0.0;
		for (auto& value : vec)
		{
			returnValue += callable(value);
		}
		return returnValue;
	}

	TaskMetrics BenchmarkRunner::runTask(Task& task)
	{
		task.applyAffinity();
		task.prepare();

		auto start = Timer::now();
		task.execute();
		auto end = Timer::now();

		task.finalize();



		//auto outputValue = task.getOutput().getSize();
		auto outputValue = accumulateOutputValues(task.getOutput().getTaskData(),
			[](const TaskData* data)
			{
				return data->getSize();
			});

		auto inputValue = accumulateOutputValues(task.getInput().getTaskData(),
			[](const TaskData* data)
			{
				return data->getSize();
			});
		auto metrics = TaskMetrics(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start),
			inputValue,
			outputValue);

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