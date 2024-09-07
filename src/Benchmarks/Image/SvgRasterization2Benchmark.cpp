//
// Created by klapeto on 7/9/2024.
//

#include "SvgRasterization2Benchmark.hpp"

#include "CommonTasks/ParseXmlTask.hpp"
#include "SvgParseTask.hpp"
#include "SvgCalculateTask.hpp"
#include "SvgDocumentGenerationTask.hpp"
#include "SvgRasterization2Task.hpp"
#include "ConvertToUInt8Task.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "PngEncodingTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> SvgRasterization2Benchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input file", ConfigurationType::File, "./assets/Elpida_Background.svg"),
				TaskConfiguration("SVG output scale", ConfigurationType::Float, "1.0"),
				TaskConfiguration("SVG rasterization sub samples", ConfigurationType::Integer, "32"),
				TaskConfiguration("Multithreaded internal operations", ConfigurationType::Boolean, "true"),
				TaskConfiguration("Output Png file", ConfigurationType::File, "./test-data-single.png"),
		};
	}

	std::vector<std::unique_ptr<Task>> SvgRasterization2Benchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());
		returnTasks.push_back(CreateTask<SvgParseTask>());
		returnTasks.push_back(CreateTask<SvgCalculateTask>(configuration.at(1).AsFloat()));
		returnTasks.push_back(
				CreateTask<SvgRasterization2Task>(configuration.at(2).AsInteger(), configuration.at(3).AsBoolean()));
		returnTasks.push_back(CreateTask<ConvertToUInt8Task>());
		returnTasks.push_back(CreateTask<PngEncodingTask>());
		returnTasks.push_back(CreateTask<FileWriteTask>(configuration.at(4).GetValue()));

		return returnTasks;
	}

	void SvgRasterization2Benchmark::DoGetBenchmarkInfo(
			String& name,
			String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Svg Rasterization 2";
		description = "Rasterizes a calculated Svg document. Uses multiple buffers instead of a single.";
		taskToUseAsScoreIndex = 4;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
		taskInfos.push_back(SvgParseTask().GetInfo());
		taskInfos.push_back(SvgCalculateTask(1.0).GetInfo());
		taskInfos.push_back(SvgRasterization2Task(1, false).GetInfo());
		taskInfos.push_back(ConvertToUInt8Task().GetInfo());
		taskInfos.push_back(PngEncodingTask().GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());
	}
} // Elpida