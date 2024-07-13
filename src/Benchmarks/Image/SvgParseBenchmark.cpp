//
// Created by klapeto on 7/4/2024.
//

#include "SvgParseBenchmark.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/ParseXmlTask.hpp"
#include "SvgParseTask.hpp"
#include "SvgCalculateTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{

	std::vector<TaskConfiguration> SvgParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input SVG file", ConfigurationType::File, "./test-data.svg")
		};
	}

	std::vector<std::unique_ptr<Task>>
	SvgParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());
		returnTasks.push_back(CreateTask<SvgParseTask>());

		return returnTasks;
	}

	void SvgParseBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& coreTaskIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Svg Parsing";
		description = "Parses an xml parsed document to SVG Dom tree.";
		coreTaskIndex = 2;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
		taskInfos.push_back(SvgParseTask().GetInfo());
	}
} // Elpida