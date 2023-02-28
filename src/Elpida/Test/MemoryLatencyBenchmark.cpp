//
// Created by klapeto on 27/2/2023.
//

#include "MemoryLatencyBenchmark.hpp"
#include "MemoryLatencyTask.hpp"

namespace Elpida
{
	std::vector<std::unique_ptr<Task>> MemoryLatencyBenchmark::GetTasks() const
	{
		std::vector<std::unique_ptr<Task>> tasks;
		tasks.push_back(std::make_unique<MemoryLatencyTask>(1024 * 1024 * 128, 64, 4096));
		return tasks;
	}

	double MemoryLatencyBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
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
} // Elpida