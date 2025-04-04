#include "StrtoulBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "StrtoulTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> StrtoulBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> StrtoulBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<StrtoulTask>());

		return returnTasks;
	}

	void StrtoulBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "strtoul()";
		description = "Measures the strtoul() throughput";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(StrtoulTask().GetInfo());
	}
} // Elpida