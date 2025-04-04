//
// Created by klapeto on 27/2/2023.
//

#include "MemoryLatencyBenchmark.hpp"
#include "MemoryLatencyTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/AllocateMemoryTask.hpp"

#include <algorithm>

namespace Elpida
{
	Vector<UniquePtr<Task>>
	MemoryLatencyBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		Size cacheLineSize = 16;
		Size pageSize = 32;

		for (auto& processor: context.GetTargetProcessors())
		{
			auto cache = processor.get().GetLastCache();
			if (cache.has_value())
			{
				cacheLineSize = std::max(cacheLineSize, cache->get().GetLineSize());
			}

			auto& numaDomain = processor.get().GetNumaNode();
			pageSize = std::max(pageSize, numaDomain.GetMemoryPageTypes().front().GetPageSize());
		}

		Vector<UniquePtr<Task>> tasks;
		tasks.push_back(CreateTask<AllocateMemoryTask>(context.GetConfiguration().at(0).AsInteger()));
		tasks.push_back(CreateTask<MemoryLatencyTask>(cacheLineSize, pageSize));
		return tasks;
	}

	Vector<TaskConfiguration> MemoryLatencyBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Memory size", ConfigurationType::Integer, "64"),
		};
	}

	void MemoryLatencyBenchmark::DoGetBenchmarkInfo(
			String& name, String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Memory latency";
		description = "Calculates the average memory access latency by randomly reading memory regions.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(AllocateMemoryTask(1).GetInfo());
		taskInfos.push_back(MemoryLatencyTask(0,0).GetInfo());
	}
} // Elpida