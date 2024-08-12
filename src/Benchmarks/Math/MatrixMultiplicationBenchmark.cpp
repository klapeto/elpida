//
// Created by klapeto on 13/8/2024.
//

#include "MatrixMultiplicationBenchmark.hpp"

#include "Elpida/Core/BenchmarkRunContext.hpp"
#include "MatrixMultiplicationTask.hpp"

namespace Elpida
{
	std::vector<TaskConfiguration> MatrixMultiplicationBenchmark::GetRequiredConfiguration() const
	{
		return {
				TaskConfiguration("Matrix rows", ConfigurationType::Integer, "32"),
				TaskConfiguration("Matrix columns", ConfigurationType::Integer, "32")
		};
	}

	std::vector<std::unique_ptr<Task>> MatrixMultiplicationBenchmark::GetTasks(BenchmarkRunContext& context) const
	{
		std::vector<std::unique_ptr<Task>> returnTasks;

		returnTasks.push_back(CreateTask<MatrixMultiplicationTask>(context.GetConfiguration().at(0).AsInteger(),
				context.GetConfiguration().at(1).AsInteger()));

		return returnTasks;
	}

	void MatrixMultiplicationBenchmark::DoGetBenchmarkInfo(String& name, String& description,
			size_t& taskToUseAsScoreIndex, std::vector<TaskInfo>& taskInfos) const
	{
		name = "Matrix multiplication";
		description = "Calculates the product between 2 matrices";
		taskToUseAsScoreIndex = 0;

		taskInfos.push_back(MatrixMultiplicationTask(0, 0).GetInfo());
	}
} // Elpida