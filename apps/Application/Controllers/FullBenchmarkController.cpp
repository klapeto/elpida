//
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkController.hpp"

#include "Models/SystemInfo/TopologyModel.hpp"
#include "Models/SystemInfo/TimingModel.hpp"

#include "Core/BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	FullBenchmarkController::FullBenchmarkController(FullBenchmarkModel& model,
			const TopologyModel& topologyModel,
			const TimingModel& overheadsModel,
			BenchmarkExecutionService& benchmarkExecutionService) :
			Controller(model),
			_topologyModel(topologyModel),
			_overheadsModel(overheadsModel),
			_benchmarkExecutionService(benchmarkExecutionService)
	{
	}

	void FullBenchmarkController::Run()
	{
		std::vector<std::size_t> affinity;

		affinity.reserve(_topologyModel.GetLeafNodes().size());
		for (auto& node: _topologyModel.GetLeafNodes())
		{
			affinity.push_back(node.get().GetOsIndex().value());
		}

	}
} // Elpida
// Application