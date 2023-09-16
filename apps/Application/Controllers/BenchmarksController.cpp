//
// Created by klapeto on 16/4/2023.
//

#include <future>
#include <string>
#include "BenchmarksController.hpp"
#include "Elpida/Core/ElpidaException.hpp"
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
		BenchmarkExecutionService& benchmarkExecutionService)
		: Controller<BenchmarksModel>(model), _topologyModel(topologyModel), _overheadsModel(overheadsModel),
		  _benchmarkResultsModel(benchmarkResultsModel),
		  _benchmarkExecutionService(benchmarkExecutionService), _cancelling(false)
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
		_cancelling = false;
		auto selectedBenchmark = _model.GetSelectedBenchmark();
		if (selectedBenchmark == nullptr)
		{
			throw ElpidaException("You have to select a benchmark first.");
		}

		if (_topologyModel.GetSelectedLeafNodes().empty())
		{
			throw ElpidaException("You have to select a the target processors first.");
		}

		try
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

			auto serializedResult = _benchmarkExecutionService.Execute(
				selectedBenchmark->GetFilePath(),
				selectedBenchmark->GetIndex(),
				affinity,
				configuration,
				std::chrono::duration_cast<NanoSeconds>(
					_overheadsModel.GetNowOverhead()).count(),
				std::chrono::duration_cast<NanoSeconds>(
					_overheadsModel.GetLoopOverhead()).count(),
				std::chrono::duration_cast<NanoSeconds>(
					_overheadsModel.GetVirtualCallOverhead()).count());

			nlohmann::json json = nlohmann::json::parse(serializedResult);

			auto score = json["score"].template get<double>();
			auto taskResultsJ = json["taskResults"];

			std::vector<TaskResultModel> taskResults;
			taskResults.reserve(taskResultsJ.size());

			for (auto& taskJ : taskResultsJ)
			{
				taskResults.emplace_back(
					NanoSeconds(taskJ["durationNanoseconds"].template get<double>()),
					taskJ["dataSize"].get<std::size_t>()
				);
			}

			_benchmarkResultsModel.Add(BenchmarkResultModel(*selectedBenchmark, score, std::move(taskResults)));
		}
		catch (const ElpidaException& ex)
		{
			if (!_cancelling)
			{
				throw;
			}
		}
	}
	void BenchmarksController::StopRunning()
	{
		_cancelling = true;
		_benchmarkExecutionService.StopCurrentExecution();
	}
} // Application