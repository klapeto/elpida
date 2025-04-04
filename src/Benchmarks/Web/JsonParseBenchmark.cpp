//
// Created by klapeto on 30/7/2024.
//

#include "JsonParseBenchmark.hpp"
#include "JsonGenerationTask.hpp"
#include "ParseJsonTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> JsonParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("File size", ConfigurationType::Integer, "128"),
		};
	}

	Vector<UniquePtr<Task>> JsonParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<JsonGenerationTask>(configuration.at(0).AsInteger()));
		returnTasks.push_back(CreateTask<ParseJsonTask>());

		return returnTasks;
	}

	void JsonParseBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Json Parsing";
		description = "Parses a Json document to determine the parsing speed.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(JsonGenerationTask(1).GetInfo());
		taskInfos.push_back(ParseJsonTask().GetInfo());
	}
} // Elpida