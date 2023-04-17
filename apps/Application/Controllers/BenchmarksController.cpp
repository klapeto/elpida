//
// Created by klapeto on 16/4/2023.
//

#include "BenchmarksController.hpp"
#include "Models/BenchmarksModel.hpp"
#include "Models/BenchmarkConfigurationModel.hpp"
#include "Models/BenchmarkConfigurationInstanceModel.hpp"

namespace Elpida::Application
{
	BenchmarksController::BenchmarksController(BenchmarksModel& model, BenchmarkConfigurationModel& configurationModel)
		: Controller<BenchmarksModel>(model), _configurationModel(configurationModel)
	{

	}

	void BenchmarksController::SetCurrentBenchmark(const BenchmarkModel* currentBenchmark)
	{
		_configurationModel.Clear();
		_model.SetSelectedBenchmark(currentBenchmark);
		if (currentBenchmark != nullptr)
		{
			for (auto& config: currentBenchmark->GetConfigurations())
			{
				_configurationModel.Add(BenchmarkConfigurationInstanceModel(config.GetName(), config.GetValue(), config.GetType()));
			}
		}
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