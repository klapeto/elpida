#include "StrtodBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "StrtodTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> StrtodBenchmark::GetRequiredConfiguration() const
	{
		return {
				// TaskConfiguration("File size", ConfigurationType::Integer, "128"),
		};
	}

	Vector<UniquePtr<Task>> StrtodBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<StrtodTask>());

		return returnTasks;
	}

	void StrtodBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "strtod()";
		description = "Measures the strtod() throughput";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(StrtodTask().GetInfo());
	}
} // Elpida