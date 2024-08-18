#[[#include]]# "${NAME}Benchmark.hpp"

#[[#include]]# "Elpida/Core/BenchmarkRunContext.hpp"
#[[#include]]# "${NAME}Task.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> ${NAME}Benchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> ${NAME}Benchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<${NAME}Task>());

		return returnTasks;
	}

	void ${NAME}Benchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "${NAME}";
		description = "${NAME} Description";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(${NAME}Task().GetInfo());
	}
} // Elpida