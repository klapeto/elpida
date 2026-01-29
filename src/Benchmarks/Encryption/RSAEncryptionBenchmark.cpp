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