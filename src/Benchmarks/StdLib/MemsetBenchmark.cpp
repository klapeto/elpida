#include "MemsetBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/AllocateMemoryTask.hpp"
#include "MemsetTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MemsetBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Memory size", ConfigurationType::Integer, "268435456")
		};
	}

	Vector<UniquePtr<Task>> MemsetBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<AllocateMemoryTask>(context.GetConfiguration().at(0).AsInteger()));
		returnTasks.push_back(CreateTask<MemsetTask>());

		return returnTasks;
	}

	void MemsetBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "memset()";
		description = "Measures the memset() throughput";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(AllocateMemoryTask(1).GetInfo());
		taskInfos.push_back(MemsetTask().GetInfo());
	}
} // Elpida