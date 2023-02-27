//
// Created by klapeto on 27/2/2023.
//

#include "Benchmark.hpp"
#include "TaskData.hpp"

namespace Elpida
{

	BenchmarkResult Benchmark::Run() const
	{
		auto tasks = GetTasks();

		std::vector<TaskResult> taskResults;

		TaskData data(0);
		for (const auto& task: tasks)
		{
			task->Prepare(data);

			auto duration = task->Run();

			task->Finalize(data);

			taskResults.emplace_back(duration, data.GetSize());
		}

		return BenchmarkResult(CalculateScore(taskResults), taskResults);
	}
} // Elpida