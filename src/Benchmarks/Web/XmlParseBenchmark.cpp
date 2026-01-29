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

#include "XmlParseBenchmark.hpp"

#include "CommonTasks/ParseXmlTask.hpp"
#include "XmlGenerationTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida
{

	Vector<TaskConfiguration> XmlParseBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("File size", ConfigurationType::Integer, "128"),
		};
	}

	Vector<UniquePtr<Task>> XmlParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<XmlGenerationTask>(configuration.at(0).AsInteger()));
		returnTasks.push_back(CreateTask<ParseXmlTask>());

		return returnTasks;
	}

	void XmlParseBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "XML Parsing";
		description = "Parses an XML document to determine the parsing speed.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(XmlGenerationTask(1).GetInfo());
		taskInfos.push_back(ParseXmlTask().GetInfo());
	}

} // Elpida