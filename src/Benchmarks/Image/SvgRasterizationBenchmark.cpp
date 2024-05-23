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

	BenchmarkInfo SvgRasterizationBenchmark::GetInfo() const
	{
		SvgDocumentGenerationTask generationTask(1);
		ParseXmlTask parseXmlTask;
		SvgParseTask svgParseTask;
		SvgCalculateTask svgCalculateTask(1.0);
		SvgRasterizationTask rasterization(16);
#ifdef ENABLE_OUTPUT
		ConvertToUInt8Task convert;
		PngEncodingTask pngEncodingTask;
		FileWriteTask fileWriteTask("");
#endif
		return BenchmarkInfo(
				"Svg Rasterization",
				"Rasterizes a calculated Svg document.",
				"samples",
				"How many samples per second are rasterized on average.",
				{
						generationTask.SetMeasured(false).GetInfo(),
						parseXmlTask.SetMeasured(false).GetInfo(),
						svgParseTask.SetMeasured(false).GetInfo(),
						svgCalculateTask.SetMeasured(false).GetInfo(),
						rasterization.GetInfo(),
#ifdef ENABLE_OUTPUT
						convert.SetMeasured(false).GetInfo(),
						pngEncodingTask.SetMeasured(false).GetInfo(),
						fileWriteTask.SetMeasured(false).GetInfo(),
#endif
				});
	}

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

		returnTasks.push_back(CreateTask<SvgDocumentGenerationTask>(false, configuration.at(0).AsInteger()));
		returnTasks.push_back(CreateTask<ParseXmlTask>(false));
		returnTasks.push_back(CreateTask<SvgParseTask>(false));
		returnTasks.push_back(CreateTask<SvgCalculateTask>(false, configuration.at(1).AsFloat()));
		returnTasks.push_back(CreateTask<SvgRasterizationTask>(true, configuration.at(2).AsInteger()));
#ifdef ENABLE_OUTPUT
		returnTasks.push_back(CreateTask<ConvertToUInt8Task>(false));
		returnTasks.push_back(CreateTask<PngEncodingTask>(false));
		returnTasks.push_back(CreateTask<FileWriteTask>(false, configuration.at(3).GetValue()));
#endif

		return returnTasks;
	}

	double SvgRasterizationBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		return taskResults[0].GetDataSize() / taskResults[0].GetDuration().count();
	}
} // Elpida