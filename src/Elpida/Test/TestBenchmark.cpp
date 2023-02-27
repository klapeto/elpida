//
// Created by klapeto on 27/2/2023.
//

#include "TestBenchmark.hpp"
#include "MemoryLatencyTask.hpp"

namespace Elpida
{
	std::vector<std::unique_ptr<Task>> TestBenchmark::GetTasks() const
	{
		std::vector<std::unique_ptr<Task>> tasks;
		tasks.push_back(std::make_unique<MemoryLatencyTask>(16768, 64, 4096));
		return tasks;
	}

	double TestBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		return taskResults.front().GetDuration().count();
	}

	BenchmarkInfo TestBenchmark::GetInfo() const
	{
		MemoryLatencyTask task(0, 0, 0);
		return BenchmarkInfo(
			"Memory latency",
			"Calculates the average memory access latency by randomly reading memory regions.",
			"",
			"The average access time.",
			ScoreType::Time,
			{ task.GetInfo() });
	}
} // Elpida