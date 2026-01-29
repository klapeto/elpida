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

#include "MemchrBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CommonTasks/AllocateMemoryTask.hpp"
#include "MemchrTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MemchrBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Memory size", ConfigurationType::Integer, "268435456")
		};
	}

	Vector<UniquePtr<Task>> MemchrBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<AllocateMemoryTask>(context.GetConfiguration().at(0).AsInteger()));
		returnTasks.push_back(CreateTask<MemchrTask>());

		return returnTasks;
	}

	void MemchrBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "memchr()";
		description = "Measures the memchr() throughput";
		taskToUseAsScoreIndex = 1;

		taskInfos.push_back(AllocateMemoryTask(1).GetInfo());
		taskInfos.push_back(MemchrTask().GetInfo());
	}
} // Elpida