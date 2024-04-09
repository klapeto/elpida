//
// Created by klapeto on 7/4/2024.
//

#include "DataParseBenchmark.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/ParseXmlTask.hpp"
#include "ParseJsonTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{
	BenchmarkInfo DataParseBenchmark::GetInfo() const
	{
		FileReadTask xmlRead("");
		ParseXmlTask xmlParse;
		ParseJsonTask jsonParse;
		return BenchmarkInfo("Web Data Parsing",
				"Measures the parsing speed of various web data exchange formats.",
				"ParseMarks",
				"The score of web parsing", {
						xmlRead.SetMeasured(false).GetInfo(),
						xmlParse.GetInfo(),
						xmlRead.SetMeasured(false).GetInfo(),
						jsonParse.GetInfo(),
				});
	}

	Vector<TaskConfiguration> DataParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input XML file", ConfigurationType::File, "./test-data.xml"),
				TaskConfiguration("Input JSON file", ConfigurationType::File, "./test-data.json"),
		};
	}

	Vector<UniquePtr<Task>> DataParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FileReadTask>(false, configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());
		returnTasks.push_back(CreateTask<FileReadTask>(false, configuration.at(1).GetValue()));
		returnTasks.push_back(CreateTask<ParseJsonTask>());

		return returnTasks;
	}

	double DataParseBenchmark::CalculateScore(const Vector<TaskResult>& taskResults) const
	{
		auto a = taskResults[0].GetDataSize() / taskResults[0].GetDuration().count();
		auto b = taskResults[1].GetDataSize() / taskResults[1].GetDuration().count();
		return (a + b) / std::mega::num;
	}
} // Elpida