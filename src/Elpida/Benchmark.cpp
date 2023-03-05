//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Benchmark.hpp"

#include "Elpida/TaskData.hpp"
#include "Elpida/ThreadTask.hpp"
#include "Elpida/ElpidaException.hpp"
#include "Elpida/TaskConfiguration.hpp"

namespace Elpida
{

	BenchmarkResult Benchmark::Run(
		const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
		const std::vector<TaskConfiguration>& configuration,
		const EnvironmentInfo& environmentInfo) const
	{
		ValidateConfiguration(configuration);
		auto tasks = GetTasks(targetProcessors, configuration, environmentInfo);

		std::vector<TaskResult> taskResults;

		TaskData data(targetProcessors.front());
		for (auto& task: tasks)
		{
			task->SetEnvironmentInfo(environmentInfo);

			if (task->CanBeMultiThreaded())
			{
				auto duration = ExecuteMultiThread(data, std::move(task), targetProcessors);
				taskResults.emplace_back(duration, data.GetSize());
			}
			else
			{
				auto duration = ExecuteSingleThread(data, std::move(task), targetProcessors.front());
				taskResults.emplace_back(duration, data.GetSize());
			}
		}

		return {
			CalculateScore(taskResults),
			taskResults
		};
	}

	Duration
	Benchmark::ExecuteSingleThread(TaskData& data, std::unique_ptr<Task> task, const TopologyNode& targetProcessor)
	{
		ThreadTask threadTask(std::move(task), targetProcessor);

		threadTask.Prepare(std::move(data));

		auto duration = threadTask.Run();

		data = std::move(threadTask.Finalize());

		return duration;
	}

	Duration
	Benchmark::ExecuteMultiThread(TaskData& data, std::unique_ptr<Task> task, const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors)
	{
		auto chunks = data.Split(targetProcessors);

		data.Deallocate();    // Reduce memory footprint

		std::vector<std::unique_ptr<ThreadTask>> threadTasks;
		for (auto& chunk: chunks)
		{
			auto threadTask = std::make_unique<ThreadTask>(task->Duplicate(), chunk.GetTargetProcessor());

			threadTask->Prepare(std::move(chunk));

			threadTasks.push_back(std::move(threadTask));
		}

		for (auto& thread: threadTasks)
		{
			thread->WakeThread();
		}

		Duration totalDuration;
		for (auto& thread: threadTasks)
		{
			totalDuration += thread->Run();
		}

		chunks.clear();

		for (auto& chunkTask: threadTasks)
		{
			chunks.push_back(std::move(chunkTask->Finalize()));
		}

		data.Merge(chunks);

		return totalDuration / threadTasks.size();
	}
	void Benchmark::ValidateConfiguration(const std::vector<TaskConfiguration>& configuration) const
	{
		auto expectedConfig = GetRequiredConfiguration();
		if (configuration.size() != expectedConfig.size())
		{
			throw ElpidaException("Benchmark configuration size was not met. It was: ", configuration.size(), " but expected ", expectedConfig.size());
		}

		for (std::size_t i = 0; i < expectedConfig.size(); ++i)
		{
			auto expected = expectedConfig[i];
			auto& actual = configuration[i];
			if (actual.GetName() != expected.GetName())
			{
				throw ElpidaException("Benchmark configuration expectation was not met. The configuration at index ", i , "should be '", expected.GetName(), "' but was '", actual.GetName());
			}

			if (actual.GetType() != expected.GetType())
			{
				throw ElpidaException("Benchmark configuration expectation was not met. The configuration at index ", i , "should be of type '", (int)expected.GetType(), "' but was '", (int)actual.GetType());
			}

			if (actual.GetValue().empty())
			{
				throw ElpidaException("Benchmark configuration expectation was not met. The configuration at index ", i , " has empty value");
			}
		}

	}
} // Elpida