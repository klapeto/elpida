/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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