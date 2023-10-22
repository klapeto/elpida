//
// Created by klapeto on 20/10/2023.
//

#include "BenchmarkRunConfigurationController.hpp"

namespace Elpida::Application
{
	BenchmarkRunConfigurationController::BenchmarkRunConfigurationController(BenchmarkRunConfigurationModel& model)
			: Controller(model)
	{

	}

	void BenchmarkRunConfigurationController::SetUploadResults(bool uploadResults)
	{
		_model.SetUploadResults(uploadResults);
	}

	void BenchmarkRunConfigurationController::SetOpenResultAfterUpload(bool openResult)
	{
		_model.SetOpenResult(openResult);
	}

	void BenchmarkRunConfigurationController::SetIterationsToRun(std::size_t iterationsToRun)
	{
		_model.SetIterationsToRun(iterationsToRun);
	}
} // Elpida
// Application