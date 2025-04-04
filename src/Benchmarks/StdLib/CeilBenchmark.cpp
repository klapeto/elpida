#include "CeilBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CeilTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> CeilBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> CeilBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<CeilTask>());

		return returnTasks;
	}

	void CeilBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "ceil()";
		description = "Measures the ceil() throughput";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(CeilTask().GetInfo());
	}
} // Elpida