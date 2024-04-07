//
// Created by klapeto on 7/4/2024.
//

#include "DataParseBenchmark.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/ParseXmlTask.hpp"

namespace Elpida
{
	BenchmarkInfo DataParseBenchmark::GetInfo() const
	{
		FileReadTask xmlRead("");
		ParseXmlTask xmlParse;
		return BenchmarkInfo("Web Data Parsing",
				"Measures the parsing speed of various web data exchange formats.",
				"ParseMarks",
				"The score of web parsing", {
						xmlRead.GetInfo(),
						xmlParse.GetInfo()
				});
	}

	Vector<TaskConfiguration> DataParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input Xml file", ConfigurationType::File, "./test-data.xml")
		};
	}

	Vector<UniquePtr<Task>> DataParseBenchmark::GetTasks(const Vector<Ref<const ProcessingUnitNode>>& targetProcessors,
			const Vector<TaskConfiguration>& configuration, const EnvironmentInfo& environmentInfo) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(std::make_unique<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(std::make_unique<ParseXmlTask>());

		return returnTasks;
	}

	double DataParseBenchmark::CalculateScore(const Vector<TaskResult>& taskResults) const
	{
		return taskResults[0].GetDataSize() / taskResults[0].GetDuration().count();
	}
} // Elpida