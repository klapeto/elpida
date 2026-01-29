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

#include "MatrixInverse4Benchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "MatrixInverse4Task.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MatrixInverse4Benchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> MatrixInverse4Benchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<MatrixInverse4Task>());

		return returnTasks;
	}

	void MatrixInverse4Benchmark::DoGetBenchmarkInfo(String& name, String& description, size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Matrix inverse (4x4)";
		description = "Calculates the inverse of a 4x4 matrix";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(MatrixInverse4Task().GetInfo());
	}
} // Elpida