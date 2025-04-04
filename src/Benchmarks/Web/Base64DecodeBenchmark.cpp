//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Base64DecodeBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "Base64DecodeTask.hpp"
#include "Base64EncodeTask.hpp"
#include "CommonTasks/GenerateLoremIpsum.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> Base64DecodeBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input size", ConfigurationType::Integer, "4096"),
		};
	}

	Vector<UniquePtr<Task>> Base64DecodeBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<GenerateLoremIpsum>(configuration.at(0).AsInteger()));
		returnTasks.push_back(CreateTask<Base64EncodeTask>());
		returnTasks.push_back(CreateTask<Base64DecodeTask>());

		return returnTasks;
	}

	void Base64DecodeBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Base64 Decode";
		description = "Decodes base64 encoded data";
		taskToUseAsScoreIndex = 2;

		taskInfos.push_back(GenerateLoremIpsum(1).GetInfo());
		taskInfos.push_back(Base64EncodeTask().GetInfo());
		taskInfos.push_back(Base64DecodeTask().GetInfo());
	}
} // Elpida