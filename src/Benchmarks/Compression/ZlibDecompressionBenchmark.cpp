//
// Created by klapeto on 31/7/2024.
//

#include "ZlibDecompressionBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "ZlibDecompressionTask.hpp"

namespace Elpida
{

	Vector<TaskConfiguration> ZlibDecompressionBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input file", ConfigurationType::File, "./Assets/file.input"),
				TaskConfiguration("Output file", ConfigurationType::File, "./file.out.input"),
		};
	}

	Vector<UniquePtr<Task>>
	ZlibDecompressionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		Vector<UniquePtr<Task>> tasks;

		tasks.push_back(CreateTask<FileReadTask>(context.GetConfiguration().at(0).GetValue()));
		tasks.push_back(CreateTask<ZlibDecompressionTask>());
		tasks.push_back(CreateTask<FileWriteTask>(context.GetConfiguration().at(1).GetValue()));
		return tasks;
	}

	void ZlibDecompressionBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Zlib decompression";
		description = "Decompresses a file with zlib";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(ZlibDecompressionTask().GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());
	}
} // Elpida