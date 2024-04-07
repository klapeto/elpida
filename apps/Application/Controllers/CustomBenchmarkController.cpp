//
// Created by klapeto on 16/4/2023.
//


#include "CustomBenchmarkController.hpp"

#include "Elpida/Core/ElpidaException.hpp"

#include "Models/Custom/CustomBenchmarkModel.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"
#include "Models/Custom/CustomBenchmarkResultsModel.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"

#include <future>
#include <string>

namespace Elpida::Application
{
	CustomBenchmarkController::CustomBenchmarkController(CustomBenchmarkModel& model,
		TopologyModel& topologyModel,
		TimingModel& overheadsModel,
		CustomBenchmarkResultsModel& benchmarkResultsModel,
		BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
		BenchmarkExecutionService& benchmarkExecutionService)
		: Controller<CustomBenchmarkModel>(model),
		  _topologyModel(topologyModel),
		  _overheadsModel(overheadsModel),
		  _benchmarkResultsModel(benchmarkResultsModel),
		  _benchmarkRunConfigurationModel(benchmarkRunConfigurationModel),
		  _benchmarkExecutionService(benchmarkExecutionService),
		  _cancelling(false)
	{

	}

	void CustomBenchmarkController::SetCurrentBenchmark(const BenchmarkModel* currentBenchmark)
	{
		_model.SetSelectedBenchmark(currentBenchmark);
	}

	void CustomBenchmarkController::Run()
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

			_benchmarkResultsModel.Add(_benchmarkExecutionService.Execute(
				*selectedBenchmark,
				affinity,
				std::chrono::duration_cast<NanoSeconds>(
					_overheadsModel.GetNowOverhead()).count(),
				std::chrono::duration_cast<NanoSeconds>(
					_overheadsModel.GetLoopOverhead()).count(),
				std::chrono::duration_cast<NanoSeconds>(
					_overheadsModel.GetVirtualCallOverhead()).count(),
				_benchmarkRunConfigurationModel.IsNumaAware(),
					_benchmarkRunConfigurationModel.IsPinThreads()));
		}
		catch (const ElpidaException& ex)
		{
			if (!_cancelling)
			{
				throw;
			}
		}
	}
	void CustomBenchmarkController::StopRunning()
	{
		_cancelling = true;
		_benchmarkExecutionService.StopCurrentExecution();
	}
} // Application