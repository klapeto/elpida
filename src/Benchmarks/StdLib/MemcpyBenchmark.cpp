#include "MemcpyBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/AllocateMemoryTask.hpp"
#include "MemcpyTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MemcpyBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Memory size", ConfigurationType::Integer, "268435456")
		};
	}

	Vector<UniquePtr<Task>> MemcpyBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<AllocateMemoryTask>(context.GetConfiguration().at(0).AsInteger()));
		returnTasks.push_back(CreateTask<MemcpyTask>());

		return returnTasks;
	}

	void MemcpyBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "memcpy()";
		description = "Measures the memcpy() throughput";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(AllocateMemoryTask(1).GetInfo());
		taskInfos.push_back(MemcpyTask().GetInfo());
	}
} // Elpida