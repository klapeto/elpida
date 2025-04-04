//
// Created by klapeto on 2/3/2023.
//

#include "PngEncodingBenchmark.hpp"

#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "PngDecodingTask.hpp"
#include "PngEncodingTask.hpp"
#include "ConvertToFloatTask.hpp"
#include "ConvertToGrayscaleTask.hpp"
#include "ConvertToUInt8Task.hpp"
#include "Elpida/Core/TaskConfiguration.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{
	std::vector<std::unique_ptr<Task>> PngEncodingBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(std::make_unique<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(std::make_unique<PngDecodingTask>());
		returnTasks.push_back(std::make_unique<PngEncodingTask>());
		returnTasks.push_back(std::make_unique<FileWriteTask>(configuration.at(1).GetValue()));

		return returnTasks;
	}

	std::vector<TaskConfiguration> PngEncodingBenchmark::GetRequiredConfiguration() const
	{
		return {
			TaskConfiguration("Input PNG file", ConfigurationType::File, "./testImage.png"),
			TaskConfiguration("Output PNG file", ConfigurationType::File, "./testImage.out.png"),
		};
	}

	void PngEncodingBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& coreTaskIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "PngEncoding";
		description = "Encodes an image file to PNG.";
		coreTaskIndex = 2;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(PngDecodingTask().GetInfo());
		taskInfos.push_back(PngEncodingTask().GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());

	}
} // Elpida