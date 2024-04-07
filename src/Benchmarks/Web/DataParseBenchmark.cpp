//
// Created by klapeto on 7/4/2024.
//

#include "DataParseBenchmark.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/ParseXmlTask.hpp"
#include "ParseJsonTask.hpp"

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
						xmlRead.GetInfo(),
						xmlParse.GetInfo(),
						xmlRead.GetInfo(),
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

	Vector<UniquePtr<Task>> DataParseBenchmark::GetTasks(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration, const EnvironmentInfo& environmentInfo) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(std::make_unique<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(std::make_unique<ParseXmlTask>());
		returnTasks.push_back(std::make_unique<FileReadTask>(configuration.at(1).GetValue()));
		returnTasks.push_back(std::make_unique<ParseJsonTask>());

		return returnTasks;
	}

	double DataParseBenchmark::CalculateScore(const Vector<TaskResult>& taskResults) const
	{
		auto a = taskResults[1].GetDataSize() / taskResults[1].GetDuration().count();
		auto b = taskResults[3].GetDataSize() / taskResults[3].GetDuration().count();
		return a + b;
	}
} // Elpida