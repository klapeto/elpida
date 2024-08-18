#include "MemsetBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "MemsetTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MemsetBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> MemsetBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<MemsetTask>());

		return returnTasks;
	}

	void MemsetBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Memset";
		description = "Memset Description";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(MemsetTask().GetInfo());
	}
} // Elpida