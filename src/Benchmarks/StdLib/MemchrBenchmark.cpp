#include "MemchrBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/AllocateMemoryTask.hpp"
#include "MemchrTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MemchrBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Memory size", ConfigurationType::Integer, "268435456")
		};
	}

	Vector<UniquePtr<Task>> MemchrBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<AllocateMemoryTask>(context.GetConfiguration().at(0).AsInteger()));
		returnTasks.push_back(CreateTask<MemchrTask>());

		return returnTasks;
	}

	void MemchrBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "memchr()";
		description = "Measures the memchr() throughput";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(AllocateMemoryTask(1).GetInfo());
		taskInfos.push_back(MemchrTask().GetInfo());
	}
} // Elpida