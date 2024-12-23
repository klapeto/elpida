//
// Created by klapeto on 10/8/2024.
//

#include "RSADecryptionBenchmark.hpp"
#include "CommonTasks/GenerateLoremIpsum.hpp"
#include "RSADecryptTask.hpp"
#include "RSAEncryptTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> RSADecryptionBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	std::vector<std::unique_ptr<Task>> RSADecryptionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<GenerateLoremIpsum>(224));
		returnTasks.push_back(CreateTask<RSAEncryptTask>());
		returnTasks.push_back(CreateTask<RSADecryptTask>());

		return returnTasks;
	}

	void RSADecryptionBenchmark::DoGetBenchmarkInfo(
			String& name,
			String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "RSA Decryption";
		description = "Decrypts data with a private key.";
		taskToUseAsScoreIndex = 2;

		taskInfos.push_back(GenerateLoremIpsum(256).GetInfo());
		taskInfos.push_back(RSAEncryptTask().GetInfo());
		taskInfos.push_back(RSADecryptTask().GetInfo());
	}
} // Elpida