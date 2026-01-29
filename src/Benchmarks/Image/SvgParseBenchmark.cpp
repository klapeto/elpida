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
// Created by klapeto on 7/4/2024.
//

#include "SvgParseBenchmark.hpp"
#include "CommonTasks/FileReadTask.hpp"
#include "CommonTasks/ParseXmlTask.hpp"
#include "SvgParseTask.hpp"
#include "SvgCalculateTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{

	std::vector<TaskConfiguration> SvgParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Input SVG file", ConfigurationType::File, "./test-data.svg")
		};
	}

	std::vector<std::unique_ptr<Task>>
	SvgParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<FileReadTask>(configuration.at(0).GetValue()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());
		returnTasks.push_back(CreateTask<SvgParseTask>());

		return returnTasks;
	}

	void SvgParseBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& coreTaskIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Svg Parsing";
		description = "Parses an xml parsed document to SVG Dom tree.";
		coreTaskIndex = 2;

		taskInfos.push_back(FileReadTask("").GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
		taskInfos.push_back(SvgParseTask().GetInfo());
	}
} // Elpida