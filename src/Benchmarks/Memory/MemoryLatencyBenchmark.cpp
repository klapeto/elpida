//
// Created by klapeto on 27/2/2023.
//

#include "MemoryLatencyBenchmark.hpp"
#include "MemoryLatencyTask.hpp"

namespace Elpida
{
	Vector<UniquePtr<Task>>
	MemoryLatencyBenchmark::GetTasks(
		const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
		const Vector<TaskConfiguration>& configuration,
		const EnvironmentInfo& environmentInfo) const
	{
		Size cacheSize = 16 * 1024 * 1024;
		Size cacheLineSize = 16;
		Size pageSize = 32;

		for (auto& processor: targetProcessors)
		{
			auto cache = processor.get().GetLastCache();
			if (cache.has_value())
			{
				cacheSize = std::max(cacheSize, cache->get().GetSize());
				cacheLineSize = std::max(cacheLineSize, cache->get().GetLineSize());
			}

			auto& numaDomain = processor.get().GetNumaNode();
			pageSize = std::max(pageSize, numaDomain.GetMemoryPageTypes().front().GetPageSize());
		}

		Vector<UniquePtr<Task>> tasks;
		tasks.push_back(std::make_unique<MemoryLatencyTask>(cacheSize * 8, cacheLineSize, pageSize));
		return tasks;
	}

	double MemoryLatencyBenchmark::CalculateScore(const Vector<TaskResult>& taskResults) const
	{
		return taskResults.front().GetDuration().count();
	}

	BenchmarkInfo MemoryLatencyBenchmark::GetInfo() const
	{
		MemoryLatencyTask task(0, 0, 0);
		return BenchmarkInfo(
			"Memory latency",
			"Calculates the average memory access latency by randomly reading memory regions.",
			"s",
			"The average access time.",
			{ task.GetInfo() });
	}

	Vector<TaskConfiguration> MemoryLatencyBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}
} // Elpida