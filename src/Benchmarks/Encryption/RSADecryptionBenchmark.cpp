//
// Created by klapeto on 10/8/2024.
//

#include "RSADecryptionBenchmark.hpp"
#include "RSADecryptTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> RSADecryptionBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	std::vector<std::unique_ptr<Task>> RSADecryptionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

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
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(RSADecryptTask().GetInfo());
	}
} // Elpida