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

#include "RayTracingBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "RayTracingTask.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> RayTracingBenchmark::GetRequiredConfiguration() const
	{
		return {
			TaskConfiguration("Render size", ConfigurationType::Integer, "100")
		};
	}

	Vector<UniquePtr<Task>> RayTracingBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		auto& configuration = context.GetConfiguration();

		returnTasks.push_back(CreateTask<RayTracingTask>(configuration.at(0).AsInteger()));

		return returnTasks;
	}

	void RayTracingBenchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "3D Ray Tracing";
		description = "Ray traces a 3D scene to rasterized image";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(RayTracingTask(100).GetInfo());
	}
} // Elpida