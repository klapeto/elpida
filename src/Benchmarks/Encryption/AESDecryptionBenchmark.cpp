//
// Created by klapeto on 11/8/2024.
//

#include "AESDecryptionBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/FileWriteTask.hpp"
#include "AESDecryptTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> AESDecryptionBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input file", ConfigurationType::File, "./lorem-ipsum.enc"),
				TaskConfiguration("Decryption key", ConfigurationType::String, "ab909b43d4cc43388ed2c98d261b082"),
				TaskConfiguration("Output file", ConfigurationType::File, "./lorem-ipsum.dec"),
		};
	}

	std::vector<std::unique_ptr<Task>> AESDecryptionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<FileReadTask>(context.GetConfiguration().at(0).GetValue()));
		returnTasks.push_back(CreateTask<AESDecryptTask>(context.GetConfiguration().at(1).GetValue()));
		returnTasks.push_back(CreateTask<FileWriteTask>(context.GetConfiguration().at(2).GetValue()));

		return returnTasks;
	}

	void AESDecryptionBenchmark::DoGetBenchmarkInfo(
			String& name,
			String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "AES Decryption";
		description = "Decrypts data with a AES";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(AESDecryptTask("").GetInfo());
		taskInfos.push_back(FileWriteTask("").GetInfo());
	}
} // Elpida