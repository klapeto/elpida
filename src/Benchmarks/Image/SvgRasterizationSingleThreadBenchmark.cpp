//
// Created by klapeto on 8/4/2024.
//

#include "SvgRasterizationSingleThreadBenchmark.hpp"
#include "CommonTasks/FileReadTask.hpp"

#include "CommonTasks/ParseXmlTask.hpp"
#include "SvgParseTask.hpp"
#include "SvgCalculateTask.hpp"
#include "SvgRasterizationSingleThreadedTask.hpp"
#include "ConvertToUInt8Task.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"

#define ENABLE_OUTPUT

#ifdef ENABLE_OUTPUT
#include "CommonTasks/FileWriteTask.hpp"
#include "PngEncodingTask.hpp"
#endif

namespace Elpida
{
	BenchmarkInfo SvgRasterizationSingleThreadBenchmark::GetInfo() const
	{

		FileReadTask read("");
		ParseXmlTask parseXmlTask;
		SvgParseTask svgParseTask;
		SvgCalculateTask svgCalculateTask(1.0);
		SvgRasterizationSingleThreadedTask rasterization(16);
#ifdef ENABLE_OUTPUT
		ConvertToUInt8Task convert;
		PngEncodingTask pngEncodingTask;
		FileWriteTask fileWriteTask("");
#endif
		return BenchmarkInfo(
				"Svg Rasterization (Single Thread)",
				"Rasterizes an calculated Svg document using 1 thread.",
				"samples",
				"How many samples per second are rasterized on average.",
				{
						read.SetMeasured(false).GetInfo(),
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

	std::vector<TaskConfiguration> SvgRasterizationSingleThreadBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input SVG file", ConfigurationType::File, "./assets/svg-rasterization.single.svg"),
				TaskConfiguration("SVG output scale", ConfigurationType::Float, "1.0"),
				TaskConfiguration("SVG rasterization sub samples", ConfigurationType::Integer, "16"),
#ifdef ENABLE_OUTPUT
				TaskConfiguration("Output Png file", ConfigurationType::File, "./test-data-single.png"),
#endif
		};
	}

	std::vector<std::unique_ptr<Task>> SvgRasterizationSingleThreadBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FileReadTask>(false, configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<ParseXmlTask>(false));
		returnTasks.push_back(CreateTask<SvgParseTask>(false));
		returnTasks.push_back(CreateTask<SvgCalculateTask>(false, configuration.at(1).AsFloat()));
		returnTasks.push_back(CreateTask<SvgRasterizationSingleThreadedTask>(true, configuration.at(2).AsInteger()));
#ifdef ENABLE_OUTPUT
		returnTasks.push_back(CreateTask<ConvertToUInt8Task>(false));
		returnTasks.push_back(CreateTask<PngEncodingTask>(false));
		returnTasks.push_back(CreateTask<FileWriteTask>(false, configuration.at(3).GetValue()));
#endif

		return returnTasks;
	}

	double SvgRasterizationSingleThreadBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		return taskResults[0].GetDataSize() / taskResults[0].GetDuration().count();
	}
} // Elpida