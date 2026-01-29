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

#include "ZlibDecompressionBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/GenerateLoremIpsum.hpp"
#include "ZlibDecompressionTask.hpp"
#include "ZlibCompressionTask.hpp"

namespace Elpida
{

	Vector<TaskConfiguration> ZlibDecompressionBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Data size", ConfigurationType::Integer, "32768")
		};
	}

	Vector<UniquePtr<Task>>
	ZlibDecompressionBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		Vector<UniquePtr<Task>> tasks;

		tasks.push_back(CreateTask<GenerateLoremIpsum>(context.GetConfiguration().at(0).AsInteger()));
		tasks.push_back(CreateTask<ZlibCompressionTask>());
		tasks.push_back(CreateTask<ZlibDecompressionTask>());
		return tasks;
	}

	void ZlibDecompressionBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Zlib decompression";
		description = "Decompresses a data with zlib";
		taskToUseAsScoreIndex = 2;

		taskInfos.push_back(GenerateLoremIpsum(4).GetInfo());
		taskInfos.push_back(ZlibCompressionTask().GetInfo());
		taskInfos.push_back(ZlibDecompressionTask().GetInfo());
	}
} // Elpida