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
// Created by klapeto on 31/7/2024.
//

#include "ZlibCompressionBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/GenerateRandomDataTask.hpp"
#include "CommonTasks/GenerateLoremIpsum.hpp"
#include "ZlibCompressionTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> ZlibCompressionBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Data size", ConfigurationType::Integer, "32768")
		};
	}

	Vector<UniquePtr<Task>>
	ZlibCompressionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		Vector<UniquePtr<Task>> tasks;

		tasks.push_back(CreateTask<GenerateLoremIpsum>(context.GetConfiguration().at(0).AsInteger()));
		tasks.push_back(CreateTask<ZlibCompressionTask>());
		return tasks;
	}

	void ZlibCompressionBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Zlib compression";
		description = "Compresses a data with zlib";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(GenerateLoremIpsum(56).GetInfo());
		taskInfos.push_back(ZlibCompressionTask().GetInfo());
	}
} // Elpida