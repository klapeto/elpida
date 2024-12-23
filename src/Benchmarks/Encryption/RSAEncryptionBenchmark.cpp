//
// Created by klapeto on 10/8/2024.
//

#include "RSAEncryptionBenchmark.hpp"
#include "CommonTasks/GenerateLoremIpsum.hpp"
#include "RSAEncryptTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> RSAEncryptionBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	std::vector<std::unique_ptr<Task>> RSAEncryptionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<GenerateLoremIpsum>(224));
		returnTasks.push_back(CreateTask<RSAEncryptTask>());

		return returnTasks;
	}

	void RSAEncryptionBenchmark::DoGetBenchmarkInfo(
			String& name,
			String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "RSA Encryption";
		description = "Encrypts data with a public key.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(GenerateLoremIpsum(256).GetInfo());
		taskInfos.push_back(RSAEncryptTask().GetInfo());
	}
} // Elpida