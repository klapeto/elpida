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
// Created by klapeto on 13/8/2024.
//

#include "DynamicMatrixMultiplicationBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "DynamicMatrixMultiplicationTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> DynamicMatrixMultiplicationBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Matrix rows", ConfigurationType::Integer, "32"),
				TaskConfiguration("Matrix columns", ConfigurationType::Integer, "32")
		};
	}

	std::vector<std::unique_ptr<Task>> DynamicMatrixMultiplicationBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<DynamicMatrixMultiplicationTask>(context.GetConfiguration().at(0).AsInteger(),
				context.GetConfiguration().at(1).AsInteger()));

		return returnTasks;
	}

	void DynamicMatrixMultiplicationBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Dynamic matrix multiplication";
		description = "Calculates the product between 2 matrices that their sizes are not known in advance";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(DynamicMatrixMultiplicationTask(0, 0).GetInfo());
	}
} // Elpida