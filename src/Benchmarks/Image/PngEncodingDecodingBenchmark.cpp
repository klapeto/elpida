//
// Created by klapeto on 2/3/2023.
//

#include "PngEncodingDecodingBenchmark.hpp"

#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "PngDecodingTask.hpp"
#include "PngEncodingTask.hpp"
#include "ConvertToFloatTask.hpp"
#include "ConvertToGrayscaleTask.hpp"
#include "ConvertToUInt8Task.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"

namespace Elpida
{
	BenchmarkInfo PngEncodingDecodingBenchmark::GetInfo() const
	{
		FileReadTask read("");
		PngDecodingTask decode;
		ConvertToFloatTask floatTask;
		ConvertToGrayscaleTask grayscaleTask;
		ConvertToUInt8Task uInt8Task;
		PngEncodingTask encode;
		FileWriteTask write("");
		return BenchmarkInfo(
			"Png decoding",
			"Decodes a stream of data from PNG decoded data to RGBA data.",
			"png_marks",
			"The input amount of bytes processed per second.",
			{
				read.GetInfo(),
				decode.GetInfo(),
				floatTask.GetInfo(),
				grayscaleTask.GetInfo(),
				uInt8Task.GetInfo(),
				encode.GetInfo(),
				write.GetInfo()
			});
	}

	std::vector<std::unique_ptr<Task>> PngEncodingDecodingBenchmark::GetTasks(
		const std::vector<std::reference_wrapper<const ProcessingUnitNode>>& targetProcessors,
		const std::vector<TaskConfiguration>& configuration,
		const EnvironmentInfo& environmentInfo) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(std::make_unique<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(std::make_unique<PngDecodingTask>());
		returnTasks.push_back(std::make_unique<ConvertToFloatTask>());
		returnTasks.push_back(std::make_unique<ConvertToGrayscaleTask>());
		returnTasks.push_back(std::make_unique<ConvertToUInt8Task>());
		returnTasks.push_back(std::make_unique<PngEncodingTask>());
		returnTasks.push_back(std::make_unique<FileWriteTask>(configuration.at(1).GetValue()));

		return returnTasks;
	}

	double PngEncodingDecodingBenchmark::CalculateScore(const std::vector<TaskResult>& taskResults) const
	{
		return static_cast<double >(taskResults[1].GetDataSize()) / taskResults[1].GetDuration().count()
			   + static_cast<double >(taskResults[2].GetDataSize()) / taskResults[2].GetDuration().count();
	}

	std::vector<TaskConfiguration> PngEncodingDecodingBenchmark::GetRequiredConfiguration() const
	{
		return {
			TaskConfiguration("Input PNG file", ConfigurationType::File, "./testImage.png"),
			TaskConfiguration("Output PNG file", ConfigurationType::File, "./testImage.out.png"),
		};
	}
} // Elpida