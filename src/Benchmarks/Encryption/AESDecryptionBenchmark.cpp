/*
 *  Copyright (c) 2024-2025  Ioannis Panagiotopoulos
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
// Created by klapeto on 11/8/2024.
//

#include "AESDecryptionBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/GenerateLoremIpsum.hpp"
#include "AESDecryptTask.hpp"
#include "AESEncryptTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> AESDecryptionBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Data size", ConfigurationType::Integer, "131072")
		};
	}

	std::vector<std::unique_ptr<Task>> AESDecryptionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<GenerateLoremIpsum>(context.GetConfiguration().at(0).AsInteger()));
		returnTasks.push_back(CreateTask<AESEncryptTask>("ab909b43d4cc43388ed2c98d261b082"));
		returnTasks.push_back(CreateTask<AESDecryptTask>("ab909b43d4cc43388ed2c98d261b082"));

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
		taskToUseAsScoreIndex = 2;

		taskInfos.push_back(GenerateLoremIpsum(15).GetInfo());
		taskInfos.push_back(AESEncryptTask("").GetInfo());
		taskInfos.push_back(AESDecryptTask("").GetInfo());
	}
} // Elpida