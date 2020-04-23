//
// Created by klapeto on 23/4/20.
//

#include "BenchmarkConfigurationController.hpp"
#include "Core/Commands/SelectedBenchmarkChangedEvent.hpp"
#include "Models/BenchmarkConfigurationsCollectionModel.hpp"
#include "Models/BenchmarkConfigurationModel.hpp"

namespace Elpida
{
	BenchmarkConfigurationController::BenchmarkConfigurationController(BenchmarkConfigurationsCollectionModel& benchmarkConfigurationsCollectionModel,
		BenchmarkConfigurationModel& configurationModel)
		: _benchmarkConfigurationsCollectionModel(benchmarkConfigurationsCollectionModel),
		  _configurationModel(configurationModel)
	{
	}

	void BenchmarkConfigurationController::handle(SelectedBenchmarkChangedEvent& command)
	{
		auto& currentBenchmark = command.getCurrentBenchmark();
		auto& conf = _benchmarkConfigurationsCollectionModel.get(currentBenchmark.getId());
		_configurationModel.transactional<BenchmarkConfigurationModel>([&currentBenchmark, &conf](BenchmarkConfigurationModel& model){
			model.setBenchmark(&currentBenchmark);
			model.setBenchmarkConfiguration(&conf);
		});
	}
}