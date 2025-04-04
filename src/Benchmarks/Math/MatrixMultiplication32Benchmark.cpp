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

#include "MatrixMultiplication32Benchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "MatrixMultiplication32Task.hpp"

namespace Elpida
{
	Vector<TaskConfiguration> MatrixMultiplication32Benchmark::GetRequiredConfiguration() const
	{
		return {};
	}

	Vector<UniquePtr<Task>> MatrixMultiplication32Benchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<MatrixMultiplication32Task>());

		return returnTasks;
	}

	void MatrixMultiplication32Benchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex,
			std::vector<TaskInfo>& taskInfos) const
	{
		name = "Matrix multiplication (32x32)";
		description = "Calculates the product between 2 32x23 matrices";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(MatrixMultiplication32Task().GetInfo());
	}
} // Elpida