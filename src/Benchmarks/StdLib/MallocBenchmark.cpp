#include "MallocBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "MallocTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MallocBenchmark::GetRequiredConfiguration() const
	{
		return {
				// TaskConfiguration("File size", ConfigurationType::Integer, "128"),
		};
	}

	Vector<UniquePtr<Task>> MallocBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<MallocTask>());

		return returnTasks;
	}

	void MallocBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Malloc Benchmark";
		description = "Measures the performance of stdlib malloc";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(MallocTask().GetInfo());
	}
} // Elpida