//
// Created by klapeto on 9/4/2024.
//

#include "SvgRasterizationBenchmark.hpp"

#include "CommonTasks/ParseXmlTask.hpp"
#include "SvgParseTask.hpp"
#include "SvgCalculateTask.hpp"
#include "SvgDocumentGenerationTask.hpp"
#include "SvgRasterizationTask.hpp"
#include "ConvertToUInt8Task.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

#define ENABLE_OUTPUT

#ifdef ENABLE_OUTPUT
#include "CommonTasks/FileWriteTask.hpp"
#include "PngEncodingTask.hpp"
#endif

namespace Elpida
{
	std::vector<TaskConfiguration> SvgRasterizationBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Shape count", ConfigurationType::Integer, "1"),
				TaskConfiguration("SVG output scale", ConfigurationType::Float, "1.0"),
				TaskConfiguration("SVG rasterization sub samples", ConfigurationType::Integer, "16"),
#ifdef ENABLE_OUTPUT
				TaskConfiguration("Output Png file", ConfigurationType::File, "./test-data-single.png"),
#endif
		};
	}

	std::vector<std::unique_ptr<Task>> SvgRasterizationBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<SvgDocumentGenerationTask>(configuration.at(0).AsInteger()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());
		returnTasks.push_back(CreateTask<SvgParseTask>());
		returnTasks.push_back(CreateTask<SvgCalculateTask>(configuration.at(1).AsFloat()));
		returnTasks.push_back(CreateTask<SvgRasterizationTask>(configuration.at(2).AsInteger()));
#ifdef ENABLE_OUTPUT
		returnTasks.push_back(CreateTask<ConvertToUInt8Task>());
		returnTasks.push_back(CreateTask<PngEncodingTask>());
		returnTasks.push_back(CreateTask<FileWriteTask>(configuration.at(3).GetValue()));
#endif

		return returnTasks;
	}

	void SvgRasterizationBenchmark::DoGetBenchmarkInfo(
			String& name,
			String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Svg Rasterization";
		description = "Rasterizes a calculated Svg document.";
		taskToUseAsScoreIndex = 4;

		taskInfos.push_back(SvgDocumentGenerationTask(1).GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
		taskInfos.push_back(SvgParseTask().GetInfo());
		taskInfos.push_back(SvgCalculateTask(1.0).GetInfo());
		taskInfos.push_back(SvgRasterizationTask(16).GetInfo());
#ifdef ENABLE_OUTPUT
		taskInfos.push_back(ConvertToUInt8Task().GetInfo());
		taskInfos.push_back(PngEncodingTask().GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());
#endif
	}
} // Elpida