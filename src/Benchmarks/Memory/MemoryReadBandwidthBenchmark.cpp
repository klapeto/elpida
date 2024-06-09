//
// Created by klapeto on 28/2/2023.
//

#include "MemoryReadBandwidthBenchmark.hpp"
#include "MemoryReadBandwidthTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/AllocateMemoryTask.hpp"

namespace Elpida
{
	BenchmarkInfo MemoryReadBandwidthBenchmark::GetInfo() const
	{
		AllocateMemoryTask allocate(0);
		MemoryReadBandwidthTask task;
		return BenchmarkInfo(
				"Memory read bandwidth",
				"Calculates the peak memory read bandwidth by continuously reading a memory stream.",
				"B/s",
				"The peak read bandwidth.",
				{
						allocate.SetMeasured(false).GetInfo(),
						task.GetInfo()
				});
	}

	double MemoryReadBandwidthBenchmark::CalculateScore(const Vector<TaskResult>& taskResults) const
	{
		Size totalSize = 0;
		Duration totalDuration = Seconds(0);

		for (auto& result : taskResults)
		{
			totalSize += result.GetDataSize();
			totalDuration += result.GetDuration();
		}

		return static_cast<double>(totalSize) / totalDuration.count();
	}

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

		tasks.push_back(CreateTask<AllocateMemoryTask>(false, context.GetConfiguration().at(0).AsInteger()));
		tasks.push_back(CreateTask<MemoryReadBandwidthTask>(true));
		return tasks;
	}
} // Elpida