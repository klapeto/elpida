//
// Created by klapeto on 16/4/2023.
//

#include <future>
#include "BenchmarksController.hpp"
#include "Models/BenchmarksModel.hpp"
#include "Models/TopologyModel.hpp"
#include "Models/OverheadsModel.hpp"

#include "Core/MessageService.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Core/Promise.hpp"

namespace Elpida::Application
{
	BenchmarksController::BenchmarksController(BenchmarksModel& model, TopologyModel& topologyModel,
			OverheadsModel& overheadsModel, BenchmarkExecutionService& benchmarkExecutionService,
			MessageService& messageService)
			: Controller<BenchmarksModel>(model), _topologyModel(topologyModel), _overheadsModel(overheadsModel),
			  _messageService(messageService), _benchmarkExecutionService(benchmarkExecutionService)
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
		auto selectedBenchmark = _model.GetSelectedBenchmark();
		if (selectedBenchmark == nullptr)
		{
			_messageService.ShowError("You have to select a benchmark first.");
			return;
		}

		if (_topologyModel.GetSelectedLeafNodes().empty())
		{
			_messageService.ShowError("You have to select a the target processors first.");
			return;
		}

		try
		{
			Promise([&, selectedBenchmark](){
				std::vector<std::size_t> affinity;
				affinity.reserve(_topologyModel.GetSelectedLeafNodes().size());
				for (auto& node: _topologyModel.GetSelectedLeafNodes())
				{
					affinity.push_back(node.get().GetOsIndex().value());
				}

				std::vector<std::string> configuration;
				configuration.reserve(selectedBenchmark->GetConfigurations().size());

				for (auto& config: selectedBenchmark->GetConfigurations())
				{
					configuration.emplace_back(config.GetValue());
				}


				return _benchmarkExecutionService.Execute(
						selectedBenchmark->GetFilePath(),
						selectedBenchmark->GetIndex(),
						affinity,
						configuration,
						std::chrono::duration_cast<std::chrono::nanoseconds, double>(
								_overheadsModel.GetNowOverhead()).count(),
						std::chrono::duration_cast<std::chrono::nanoseconds, double>(
								_overheadsModel.GetLoopOverhead()).count(),
						std::chrono::duration_cast<std::chrono::nanoseconds, double>(
								_overheadsModel.GetVirtualCallOverhead()).count());
			}, [&](auto value){
				_messageService.ShowMessage(value);
			});
		}
		catch (const ElpidaException& ex)
		{
			_messageService.ShowError(ex.what());
		}
	}
} // Application