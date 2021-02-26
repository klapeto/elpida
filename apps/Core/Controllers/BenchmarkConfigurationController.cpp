/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 23/4/20.
//

#include "BenchmarkConfigurationController.hpp"

#include "Core/Commands/SelectedBenchmarkChangedEvent.hpp"
#include "Models/BenchmarkConfigurationsCollectionModel.hpp"
#include "Models/BenchmarkConfigurationModel.hpp"
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

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
		auto currentBenchmark = command.getCurrentBenchmark();
		OptionalReference<BenchmarkConfiguration> conf;
		if (currentBenchmark.has_value())
		{
			conf = _benchmarkConfigurationsCollectionModel.get(currentBenchmark->get().getId());
		}

		_configurationModel
			.transactional<BenchmarkConfigurationModel>([&currentBenchmark, &conf](BenchmarkConfigurationModel& model)
			{
				model.setBenchmark(currentBenchmark);
				model.setBenchmarkConfiguration(conf);
			});

	}
}