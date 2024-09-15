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

#include "RegexBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "RegexTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> RegexBenchmark::GetRequiredConfiguration() const
	{
		return {

//				TaskConfiguration("Input file", ConfigurationType::File, "./assets/lorem-ipsum.txt"),
//				TaskConfiguration("Regex", ConfigurationType::String, "/Test/ig")
		};
	}

	Vector<UniquePtr<Task>> RegexBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		//returnTasks.push_back(CreateTask<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<RegexTask>());

		return returnTasks;
	}

	void RegexBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Regex";
		description = "Searches text with regex";
		taskToUseAsScoreIndex = 0;

		//taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(RegexTask().GetInfo());
	}
} // Elpida