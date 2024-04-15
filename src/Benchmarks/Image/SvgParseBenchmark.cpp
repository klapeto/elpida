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
	BenchmarkInfo SvgParseBenchmark::GetInfo() const
	{
		FileReadTask read("");
		ParseXmlTask parseXmlTask;
		SvgParseTask svgParseTask;
		SvgCalculateTask svgCalculateTask(1.0);
		return BenchmarkInfo(
				"Svg Parsing",
				"Parses an xml parsed document to SVG Dom tree.",
				"SvgMarks",
				"The",
				{
						read.SetMeasured(false).GetInfo(),
						parseXmlTask.SetMeasured(false).GetInfo(),
						svgParseTask.GetInfo(),
						svgCalculateTask.GetInfo(),
				});
	}

	std::vector<TaskConfiguration> SvgParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input SVG file", ConfigurationType::File, "./test-data.svg"),
				TaskConfiguration("SVG output scale", ConfigurationType::Float, "1.0")
		};
	}

	std::vector<std::unique_ptr<Task>>
	SvgParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FileReadTask>(false, configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<ParseXmlTask>(false));
		returnTasks.push_back(CreateTask<SvgParseTask>());
		returnTasks.push_back(CreateTask<SvgCalculateTask>(true, configuration.at(1).AsFloat()));

		return returnTasks;
	}

	double SvgParseBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		auto a = taskResults[0].GetDataSize() / taskResults[0].GetDuration().count();
		auto b = taskResults[1].GetDataSize() / taskResults[1].GetDuration().count();
		return (a + b);
	}
} // Elpida