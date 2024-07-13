//
// Created by klapeto on 28/2/2023.
//

#include "MemoryReadBandwidthBenchmark.hpp"
#include "MemoryReadBandwidthTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/AllocateMemoryTask.hpp"

namespace Elpida
{

	Vector<TaskConfiguration> MemoryReadBandwidthBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Memory size", ConfigurationType::Integer, "64"),
		};
	}

	Vector<UniquePtr<Task>>
	MemoryReadBandwidthBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		Vector<UniquePtr<Task>> tasks;

		tasks.push_back(CreateTask<AllocateMemoryTask>(context.GetConfiguration().at(0).AsInteger()));
		tasks.push_back(CreateTask<MemoryReadBandwidthTask>());
		return tasks;
	}

	void MemoryReadBandwidthBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Memory read bandwidth";
		description = "Calculates the peak memory read bandwidth by continuously reading a memory stream.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(AllocateMemoryTask(1).GetInfo());
		taskInfos.push_back(MemoryReadBandwidthTask().GetInfo());
	}
} // Elpida