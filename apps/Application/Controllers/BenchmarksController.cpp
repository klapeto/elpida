//
// Created by klapeto on 16/4/2023.
//

#include "BenchmarksController.hpp"
#include "Models/BenchmarksModel.hpp"

namespace Elpida::Application
{
	BenchmarksController::BenchmarksController(BenchmarksModel& model)
		: Controller<BenchmarksModel>(model)
	{

	}

	void BenchmarksController::SetCurrentBenchmark(const BenchmarkModel* currentBenchmark)
	{
		_model.SetSelectedBenchmark(currentBenchmark);
	}

	void BenchmarksController::SetUploadResults(bool uploadResults)
	{
		_model.SetUploadResults(uploadResults);
	}

	void BenchmarksController::SetOpenResultAfterUpload(bool openResult)
	{
		_model.SetOpenResult(openResult);
	}

	void BenchmarksController::SetIterationsToRun(int iterations)
	{
		_model.SetIterationsToRun(iterations);
	}
} // Application