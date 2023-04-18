//
// Created by klapeto on 16/4/2023.
//

#include "BenchmarksController.hpp"
#include "Models/BenchmarksModel.hpp"
#include "Models/TopologyModel.hpp"

#include "Core/MessageService.hpp"

namespace Elpida::Application
{
	BenchmarksController::BenchmarksController(BenchmarksModel& model, TopologyModel& topologyModel, MessageService& messageService)
		: Controller<BenchmarksModel>(model), _topologyModel(topologyModel), _messageService(messageService)
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

	void BenchmarksController::Run()
	{
		if (_model.GetSelectedBenchmark() == nullptr)
		{
			_messageService.ShowError("You have to select a benchmark first.");
		}

		if (_topologyModel.GetSelectedLeafNodes().empty())
		{
			_messageService.ShowError("You have to select a the target processors first.");
		}
	}
} // Application