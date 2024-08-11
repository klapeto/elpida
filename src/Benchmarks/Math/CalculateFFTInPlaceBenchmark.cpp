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