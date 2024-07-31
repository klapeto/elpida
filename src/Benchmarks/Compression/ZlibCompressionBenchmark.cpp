//
// Created by klapeto on 31/7/2024.
//

#include "ZlibCompressionBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "ZlibCompressionTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> ZlibCompressionBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("File", ConfigurationType::File, "./assets/lorem-ipsum.txt"),
				TaskConfiguration("Output file", ConfigurationType::File, "./lorem-ipsum.z"),
		};
	}

	Vector<UniquePtr<Task>>
	ZlibCompressionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		Vector<UniquePtr<Task>> tasks;

		tasks.push_back(CreateTask<FileReadTask>(context.GetConfiguration().at(0).GetValue()));
		tasks.push_back(CreateTask<ZlibCompressionTask>());
		tasks.push_back(CreateTask<FileWriteTask>(context.GetConfiguration().at(1).GetValue()));
		return tasks;
	}

	void ZlibCompressionBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Zlib compression";
		description = "Compresses a file with zlib";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(ZlibCompressionTask().GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());
	}
} // Elpida