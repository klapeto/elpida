//
// Created by klapeto on 27/2/2023.
//

#include "Benchmark.hpp"

#include "TaskData.hpp"
#include "ThreadTask.hpp"

namespace Elpida
{

	BenchmarkResult Benchmark::Run(const std::vector<int>& targetProcessorIds, const OverheadsInfo& overheadsInfo) const
	{
		auto tasks = GetTasks();

		std::vector<TaskResult> taskResults;

		TaskData data(0);
		for (auto& task: tasks)
		{
			task->SetOverheads(overheadsInfo);

			if (task->CanBeMultiThreaded())
			{
				auto duration = ExecuteMultiThread(data, std::move(task), targetProcessorIds);
				taskResults.emplace_back(duration, data.GetSize());
			}
			else
			{
				auto duration = ExecuteSingleThread(data, std::move(task), targetProcessorIds.front());
				taskResults.emplace_back(duration, data.GetSize());
			}
		}

		return {
			CalculateScore(taskResults),
			taskResults
		};
	}

	Duration Benchmark::ExecuteSingleThread(TaskData& data, std::unique_ptr<Task> task, int processorId)
	{
		data.Migrate(processorId); // TODO: Move it?
		ThreadTask threadTask(std::move(task), processorId);

		threadTask.Prepare(std::move(data));

		auto duration = threadTask.Run();

		data = std::move(threadTask.Finalize());

		return duration;
	}

	Duration
	Benchmark::ExecuteMultiThread(TaskData& data, std::unique_ptr<Task> task, const std::vector<int>& targetProcessorIds)
	{
		auto chunks = data.Split(targetProcessorIds);

		data.Deallocate(); 	// Reduce memory footprint

		std::vector<std::unique_ptr<ThreadTask>> threadTasks;
		for (auto& chunk: chunks)
		{
			auto threadTask = std::make_unique<ThreadTask>(task->Duplicate(), chunk.GetProcessorId());

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
} // Elpida