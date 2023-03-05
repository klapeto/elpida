//
// Created by klapeto on 28/2/2023.
//

#include "MemoryReadBandwidthBenchmark.hpp"
#include "MemoryReadBandwidthTask.hpp"

namespace Elpida
{
	BenchmarkInfo MemoryReadBandwidthBenchmark::GetInfo() const
	{
		MemoryReadBandwidthTask task(0);
		return BenchmarkInfo(
			"Memory read bandwidth",
			"Calculates the peak memory read bandwidth by continuously reading a memory stream.",
			"B/s",
			"The peak read bandwidth.",
			{ task.GetInfo() });
	}

	std::vector<std::unique_ptr<Task>>
	MemoryReadBandwidthBenchmark::GetTasks(const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
		const std::vector<TaskConfiguration>& configuration,
		const EnvironmentInfo& environmentInfo) const
	{

		std::size_t cacheSize = 16 * 1024 * 1024;

		for (auto& processor: targetProcessors)
		{
			auto cache = processor.get().GetLastCache();
			if (cache.has_value())
			{
				cacheSize = std::max(cacheSize, cache->get().GetSize());
			}
		}

		std::vector<std::unique_ptr<Task>> tasks;
		tasks.push_back(std::make_unique<MemoryReadBandwidthTask>(cacheSize * 8));
		return tasks;
	}

	double MemoryReadBandwidthBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		std::size_t totalSize = 0;
		Duration totalDuration = Seconds(0);

		for (auto& result: taskResults)
		{
			totalSize += result.GetInputSize();
			totalDuration += result.GetDuration();
		}

		return static_cast<double>(totalSize) / totalDuration.count();
	}

	std::vector<TaskConfiguration> MemoryReadBandwidthBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}
} // Elpida