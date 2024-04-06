//
// Created by klapeto on 27/2/2023.
//

#include "Elpida/Core/Benchmark.hpp"

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/RawTaskData.hpp"
#include "Elpida/Core/TaskResult.hpp"
#include "Elpida/Core/ThreadTask.hpp"
#include "Elpida/Core/ElpidaException.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"
#include "Elpida/Core/Vector.hpp"

namespace Elpida
{
	BenchmarkResult Benchmark::Run(
		const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
		const Vector<TaskConfiguration>& configuration,
		const EnvironmentInfo& environmentInfo) const
	{
		ValidateConfiguration(configuration);
		auto tasks = GetTasks(targetProcessors, configuration, environmentInfo);

		Vector<TaskResult> taskResults;

		auto allocators = environmentInfo.GetAllocatorFactory().Create({targetProcessors});
		UniquePtr<AbstractTaskData> data = std::make_unique<RawTaskData>(allocators.front());
		for (auto& task : tasks)
		{
			task->SetEnvironmentInfo(environmentInfo);
			Duration duration;
			Size processedDataSize = 0;
			if (task->CanBeMultiThreaded())
			{
				duration = ExecuteMultiThread(data, std::move(task), allocators, targetProcessors, processedDataSize,
						environmentInfo.IsPinThreads());
			}
			else
			{
				duration = ExecuteSingleThread(data, std::move(task), targetProcessors.front(), processedDataSize,
						environmentInfo.IsPinThreads());
			}

			taskResults.emplace_back(duration, processedDataSize);
		}

		auto score = CalculateScore(taskResults);
		return {
			score,
			std::move(taskResults)
		};
	}

	Duration
	Benchmark::ExecuteSingleThread(UniquePtr<AbstractTaskData>& data,
		UniquePtr<Task> task,
		const ProcessingUnitNode& targetProcessor,
		Size& processedDataSize,
		bool pinThreads)
	{
		ThreadTask threadTask(std::move(task), targetProcessor, pinThreads);

		auto allocator = data->GetAllocator();
		threadTask.Prepare(std::move(data));

		allocator->ResetStatistics();
		auto duration = threadTask.Run();

		processedDataSize = threadTask.GetProcessedDataSize();

		data = threadTask.Finalize();

		return duration;
	}

	Duration
	Benchmark::ExecuteMultiThread(UniquePtr<AbstractTaskData>& data,
		UniquePtr<Task> task,
		const Vector<SharedPtr<Allocator>>& allocators,
		const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
		Size& processedDataSize, bool pinThreads)
	{
		auto chunks = data->Split(allocators);

		data->Deallocate();    // Reduce memory footprint

		Vector<UniquePtr<ThreadTask>> threadTasks;

		for (std::size_t i = 0; i < chunks.size(); ++i)
		{
			auto threadTask = std::make_unique<ThreadTask>(task->Duplicate(), targetProcessors[i], pinThreads);

			auto  allocator = chunks[i]->GetAllocator();
			threadTask->Prepare(std::move(chunks[i]));

			allocator->ResetStatistics();
			threadTasks.push_back(std::move(threadTask));
		}

		for (auto& thread : threadTasks)
		{
			thread->WakeThread();
		}

		Duration totalDuration;
		for (auto& thread : threadTasks)
		{
			totalDuration += thread->Run();
		}

		chunks.clear();

		for (auto& chunkTask : threadTasks)
		{
			processedDataSize += chunkTask->GetProcessedDataSize();
			chunks.push_back(chunkTask->Finalize());
		}

		data->Merge(chunks);

		return totalDuration / threadTasks.size();
	}

	void Benchmark::ValidateConfiguration(const Vector<TaskConfiguration>& configuration) const
	{
		auto expectedConfig = GetRequiredConfiguration();
		if (configuration.size() != expectedConfig.size())
		{
			throw ElpidaException("Benchmark configuration size was not met. It was: ",
				configuration.size(),
				" but expected ",
				expectedConfig.size());
		}

		for (Size i = 0; i < expectedConfig.size(); ++i)
		{
			auto expected = expectedConfig[i];
			auto& actual = configuration[i];
			if (actual.GetName() != expected.GetName())
			{
				throw ElpidaException("Benchmark configuration expectation was not met. The configuration at index ",
					i,
					"should be '",
					expected.GetName(),
					"' but was '",
					actual.GetName());
			}

			if (actual.GetType() != expected.GetType())
			{
				throw ElpidaException("Benchmark configuration expectation was not met. The configuration at index ",
					i,
					"should be of type '",
					(int)expected.GetType(),
					"' but was '",
					(int)actual.GetType());
			}

			if (actual.GetValue().empty())
			{
				throw ElpidaException("Benchmark configuration expectation was not met. The configuration at index ",
					i,
					" has empty value");
			}
		}

	}
} // Elpida