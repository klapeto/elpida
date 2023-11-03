//
// Created by klapeto on 2/11/2023.
//

#include "SvgBenchmark.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "SvgParseTask.hpp"
#include "PngEncodingTask.hpp"
#include "SvgRasterization.hpp"
#include "CommonTasks/FileWriteTask.hpp"

namespace Elpida
{
	BenchmarkInfo SvgBenchmark::GetInfo() const
	{
		FileReadTask read("");
		SvgParseTask svgParse;
		//SvgRasterization svgRasterization(1.0);
		//PngEncodingTask pngEncoding;
		//FileWriteTask write("");
		return BenchmarkInfo("Svg Benchmark",
			"Measures the parsing/rasteriazation of Svg images.",
			"svgmarks",
			"",
			{
				read.GetInfo(),
				svgParse.GetInfo(),
				//svgRasterization.GetInfo(),
				//pngEncoding.GetInfo(),
				//write.GetInfo()
			});
	}
	std::vector<TaskConfiguration> SvgBenchmark::GetRequiredConfiguration() const
	{
		return {
			TaskConfiguration("Input SVG file", ConfigurationType::File, "./testImage.svg"),
			TaskConfiguration("Output PNG file", ConfigurationType::File, "./testImage.out.png"),
		};
	}

	std::vector<std::unique_ptr<Task>> SvgBenchmark::GetTasks(const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
		const std::vector<TaskConfiguration>& configuration,
		const EnvironmentInfo& environmentInfo) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(std::make_unique<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(std::make_unique<SvgParseTask>());
		//returnTasks.push_back(std::make_unique<SvgRasterization>(2.0));
		//returnTasks.push_back(std::make_unique<PngEncodingTask>());
		//returnTasks.push_back(std::make_unique<FileWriteTask>(configuration.at(1).GetValue()));

		return returnTasks;
	}

	double SvgBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		return taskResults[1].GetDataSize() / taskResults[1].GetDuration().count();
	}
} // Elpida