//
// Created by klapeto on 7/4/2024.
//

#include "XmlParseBenchmark.hpp"

#include "CommonTasks/ParseXmlTask.hpp"
#include "XmlGenerationTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{

	Vector<TaskConfiguration> XmlParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("File size", ConfigurationType::Integer, "128"),
		};
	}

	Vector<UniquePtr<Task>> XmlParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<XmlGenerationTask>(configuration.at(0).AsInteger()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());

		return returnTasks;
	}

	void XmlParseBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "XML Parsing";
		description = "Parses an XML document to determine the parsing speed.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(XmlGenerationTask(1).GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
	}

} // Elpida