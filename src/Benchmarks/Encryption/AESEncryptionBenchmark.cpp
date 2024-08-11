//
// Created by klapeto on 11/8/2024.
//

#include "AESEncryptionBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "AESEncryptTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> AESEncryptionBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input file", ConfigurationType::File, "./assets/lorem-ipsum.txt"),
				TaskConfiguration("Encryption key", ConfigurationType::String, "ab909b43d4cc43388ed2c98d261b082"),
				TaskConfiguration("Output file", ConfigurationType::File, "./lorem-ipsum.enc"),
		};
	}

	std::vector<std::unique_ptr<Task>> AESEncryptionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<FileReadTask>(context.GetConfiguration().at(0).GetValue()));
		returnTasks.push_back(CreateTask<AESEncryptTask>(context.GetConfiguration().at(1).GetValue()));
		returnTasks.push_back(CreateTask<FileWriteTask>(context.GetConfiguration().at(2).GetValue()));

		return returnTasks;
	}

	void AESEncryptionBenchmark::DoGetBenchmarkInfo(
			String& name,
			String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "AES Encryption";
		description = "Encrypts data with a AES.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(AESEncryptTask("").GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());
	}
} // Elpida