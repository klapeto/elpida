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

		try
		{

			std::vector<std::size_t> affinity;

			affinity.reserve(_topologyModel.GetSelectedLeafNodes().size());
			for (auto& node : _topologyModel.GetSelectedLeafNodes())
			{
				affinity.push_back(node.get().GetOsIndex().value());
			}

			for (std::size_t i = 0; i < _benchmarkRunConfigurationModel.GetIterationsToRun(); ++i)
			{
				_benchmarkResultsModel.Add(_benchmarkExecutionService.Execute(
						*selectedBenchmark,
						affinity,
						_overheadsModel.GetNowOverhead().count(),
						_overheadsModel.GetLoopOverhead().count(),
						_overheadsModel.GetVirtualCallOverhead().count(),
						_benchmarkRunConfigurationModel.GetDependentQueueRatio(),
						_benchmarkRunConfigurationModel.GetIndependentQueueRatio(),
						_benchmarkRunConfigurationModel.IsNumaAware(),
						_benchmarkRunConfigurationModel.IsPinThreads()));
			}
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