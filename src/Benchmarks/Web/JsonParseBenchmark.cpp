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
// Created by klapeto on 30/7/2024.
//

#include "JsonParseBenchmark.hpp"
#include "EmbeddedDocumentTask.hpp"
#include "ParseJsonTask.hpp"
#include "Elpida/Core/BenchmarkRunContext.hpp"

namespace Elpida::Benchmarks::Web
{
	Vector<TaskConfiguration> JsonParseBenchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> JsonParseBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<EmbeddedDocumentTask>(JsonBenchmarkDocument, JsonBenchmarkDocument_SIZE));
		returnTasks.push_back(CreateTask<ParseJsonTask>());

		return returnTasks;
	}

	void JsonParseBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Json Parsing";
		description = "Parses a Json document to determine the parsing speed.";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(EmbeddedDocumentTask(JsonBenchmarkDocument, JsonBenchmarkDocument_SIZE).GetInfo());
		taskInfos.push_back(ParseJsonTask().GetInfo());
	}
} // Elpida