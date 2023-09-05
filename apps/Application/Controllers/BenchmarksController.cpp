//
// Created by klapeto on 16/4/2023.
//

#include <future>
#include <string>
#include "BenchmarksController.hpp"
#include "Models/BenchmarksModel.hpp"
#include "Models/TopologyModel.hpp"
#include "Models/OverheadsModel.hpp"
#include "Models/BenchmarkResultsModel.hpp"

#include "Core/MessageService.hpp"
#include "Core/BenchmarkExecutionService.hpp"
#include "Core/Promise.hpp"
#include "json.hpp"

namespace Elpida::Application
{
	BenchmarksController::BenchmarksController(BenchmarksModel& model,
		TopologyModel& topologyModel,
		OverheadsModel& overheadsModel,
		BenchmarkResultsModel& benchmarkResultsModel,
		BenchmarkExecutionService& benchmarkExecutionService,
		MessageService& messageService)
		: Controller<BenchmarksModel>(model), _topologyModel(topologyModel), _overheadsModel(overheadsModel),
		  _benchmarkResultsModel(benchmarkResultsModel), _messageService(messageService), _benchmarkExecutionService(benchmarkExecutionService), _cancelling(false)
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

	Promise<> BenchmarksController::Run()
	{
		_cancelling = false;
		auto selectedBenchmark = _model.GetSelectedBenchmark();
		if (selectedBenchmark == nullptr)
		{
			_messageService.ShowError("You have to select a benchmark first.");
			co_return;
		}

		if (_topologyModel.GetSelectedLeafNodes().empty())
		{
			_messageService.ShowError("You have to select a the target processors first.");
			co_return;
		}

		try
		{
			auto serializedResult = co_await AsyncPromise<std::string>([this, selectedBenchmark]()
			{
			  std::vector<std::size_t> affinity;

			  affinity.reserve(_topologyModel.GetSelectedLeafNodes().size());
			  for (auto& node : _topologyModel.GetSelectedLeafNodes())
			  {
				  affinity.push_back(node.get().GetOsIndex().value());
			  }

			  std::vector<std::string> configuration;
			  configuration.reserve(selectedBenchmark->GetConfigurations().size());

			  for (auto& config : selectedBenchmark->GetConfigurations())
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
			});

			nlohmann::json json = nlohmann::json::parse(serializedResult);

			auto score = json["score"].template get<double>();
			auto taskResultsJ = json["taskResults"];

			std::vector<TaskResultModel> taskResults;
			taskResults.reserve(taskResultsJ.size());

			for (auto& taskJ : taskResultsJ)
			{
				taskResults.emplace_back(
					NanoSeconds(taskJ["durationNanoseconds"].template get<double>()),
					taskJ["inputSize"].get<std::size_t>()
				);
			}

			_benchmarkResultsModel.Add(BenchmarkResultModel(*selectedBenchmark,  score, std::move(taskResults)));
		}
		catch (const ElpidaException& ex)
		{
			if (!_cancelling)
			{
				_messageService.ShowError(ex.what());
			}
		}
	}
	void BenchmarksController::StopRunning()
	{
		_cancelling = true;
		_benchmarkExecutionService.StopCurrentExecution();
	}
} // Application