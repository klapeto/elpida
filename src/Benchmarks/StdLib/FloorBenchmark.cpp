#include "FloorBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "FloorTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> FloorBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> FloorBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FloorTask>());

		return returnTasks;
	}

	void FloorBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "floor()";
		description = "Measures the floor() throughput";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(FloorTask().GetInfo());
	}
} // Elpida