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
#include "Elpida/Core/BenchmarkRunContext.hpp"


#include <fstream>

namespace Elpida
{
	BenchmarkResult Benchmark::Run(BenchmarkRunContext& context) const
	{
		ValidateConfiguration(context.GetConfiguration());
		auto tasks = GetTasks(context);

		Vector<TaskResult> taskResults;

		auto allocators = context.GetAllocatorFactory()->Create({ context.GetTargetProcessors() });
		SharedPtr<AbstractTaskData> data = std::make_shared<RawTaskData>(allocators.front());
		for (auto& task : tasks)
		{
			task->SetEnvironmentInfo(context.GetEnvironmentInfo());
			Duration duration;
			Size processedDataSize = 0;
			bool shouldBeMeasured = task->ShouldBeMeasured();
			if (task->GetAllowedConcurrency() != ConcurrencyMode::None &&
				context.GetConcurrencyMode() != ConcurrencyMode::None)
			{
				duration = ExecuteMultiThread(context.GetConcurrencyMode(), data, std::move(task), allocators,
						context.GetTargetProcessors(), context.GetEnvironmentInfo().GetTopologyInfo(), processedDataSize, context.IsPinThreads());
			}
			else
			{
				duration = ExecuteSingleThread(data, std::move(task), processedDataSize);
			}

			if (shouldBeMeasured)
			{
				taskResults.emplace_back(duration, processedDataSize);
			}
		}

		auto score = CalculateScore(taskResults);
		return {
				score,
				std::move(taskResults)
		};
	}

	Duration
	Benchmark::ExecuteSingleThread(SharedPtr<AbstractTaskData>& data,
			UniquePtr<Task> task,
			Size& processedDataSize)
	{
		auto allocator = data->GetAllocator();
		task->Prepare(std::move(data));

		allocator->ResetStatistics();
		auto duration = task->Run();

		processedDataSize = task->GetProcessedDataSize();

		data = task->Finalize();

		return duration;
	}

	static ConcurrencyMode DeduceConcurrencyMode(const UniquePtr<Task>& task, ConcurrencyMode concurrencyMode)
	{
		if ((static_cast<int>(task->GetAllowedConcurrency()) & static_cast<int>(concurrencyMode)) != 0)
		{
			return concurrencyMode;
		}
		return task->GetAllowedConcurrency();
	}

	Duration
	Benchmark::ExecuteMultiThread(
			ConcurrencyMode concurrencyMode,
			SharedPtr<AbstractTaskData>& data,
			UniquePtr<Task> task,
			const Vector<SharedPtr<Allocator>>& allocators,
			const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const TopologyInfo& topologyInfo,
			Size& processedDataSize,
			bool pinThreads)
	{
		Vector<SharedPtr<AbstractTaskData>> inputData;

		concurrencyMode = DeduceConcurrencyMode(task, concurrencyMode);

		switch (concurrencyMode)
		{
		case ConcurrencyMode::CopyInput:
			inputData = data->Copy(allocators);
			data->Deallocate();    // Reduce memory footprint
			break;
		case ConcurrencyMode::ShareInput:
			inputData.reserve(targetProcessors.size());
			for (std::size_t i = 0; i < targetProcessors.size(); ++i)
			{
				inputData.push_back(data);
			}
			break;
		case ConcurrencyMode::ChunkInput:
			inputData = data->Split(allocators);
			data->Deallocate();    // Reduce memory footprint
			break;
		default:
			throw ElpidaException("Invalid operation: cannot call ExecuteMultiThread with ConcurrencyMode::None");
		}

		return ExecuteConcurrent(concurrencyMode, std::move(task), data, std::move(inputData), targetProcessors, topologyInfo,
				processedDataSize, pinThreads);
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

	Duration Benchmark::ExecuteConcurrent(
			ConcurrencyMode concurrencyMode,
			UniquePtr<Task> task,
			SharedPtr<AbstractTaskData>& data,
			Vector<SharedPtr<AbstractTaskData>> inputData,
			const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const TopologyInfo& topologyInfo,
			Size& processedDataSize,
			bool pinThreads)
	{
		Vector<UniquePtr<ThreadTask>> threadTasks;

		for (std::size_t i = 0; i < inputData.size(); ++i)
		{
			auto threadTask = std::make_unique<ThreadTask>(task->Duplicate(),
					pinThreads ? Optional<Ref<const ProcessingUnitNode>>(targetProcessors[i]) : std::nullopt, topologyInfo);

			auto allocator = inputData[i]->GetAllocator();
			threadTask->Prepare(std::move(inputData[i]));

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

		inputData.clear();

		for (auto& chunkTask : threadTasks)
		{
			processedDataSize += chunkTask->GetProcessedDataSize();
			inputData.push_back(chunkTask->Finalize());
		}

		switch (concurrencyMode)
		{
		case ConcurrencyMode::CopyInput:
		case ConcurrencyMode::ShareInput:
			data = inputData[0];
			break;
		case ConcurrencyMode::ChunkInput:
			data->Merge(inputData);
			break;
		default:
			throw ElpidaException("Invalid operation: cannot call ExecuteMultiThread with ConcurrencyMode::None");
		}

		// this used to be threadTasks.size() but for some weird reason I haven't figured out yet, on CI generated
		// builds it randomly returned huge random numbers when performing SVG Rast with shared input (it was like threadTasks.size() was 0 or something).
		// Not reproed localy with -O2. Valgrind shows nothing memory wise. Even tried about same compiler ver (GCC 9.5, although CI
		// has 9.3) and still not reproed. Putting something before (I tried logging that before returning it) seemed to fix that.
		return totalDuration / inputData.size();
	}
} // Elpida