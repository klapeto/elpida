#include "SqrtBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "SqrtTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> SqrtBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> SqrtBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<SqrtTask>());

		return returnTasks;
	}

	void SqrtBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "sqrt()";
		description = "Measures the sqrt() throughput";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(SqrtTask().GetInfo());
	}
} // Elpida