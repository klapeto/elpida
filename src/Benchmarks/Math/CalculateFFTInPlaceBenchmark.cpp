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
// Created by klapeto on 12/8/2024.
//

#include "CalculateFFTInPlaceBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "CalculateFFTInPlaceTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> CalculateFFTInPlaceBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Workload size in bytes", ConfigurationType::Integer, "2048")
		};
	}

	std::vector<std::unique_ptr<Task>> CalculateFFTInPlaceBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<CalculateFFTInPlaceTask>(context.GetConfiguration().at(0).AsInteger()));

		return returnTasks;
	}

	void CalculateFFTInPlaceBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "FFT calculation in place";
		description = "Calculates the DFT of values in place";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(CalculateFFTInPlaceTask(1).GetInfo());
	}
} // Elpida