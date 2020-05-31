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

#ifndef APPS_QT_CONTROLLERS_BENCHMARKCONFIGURATIONCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_BENCHMARKCONFIGURATIONCONTROLLER_HPP

#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida
{
	class BenchmarkConfigurationsCollectionModel;
	class BenchmarkConfigurationModel;

	class BenchmarkConfigurationController : public CommandHandler
	{
	public:

		void handle(SelectedBenchmarkChangedEvent& command) override;

		BenchmarkConfigurationController(BenchmarkConfigurationsCollectionModel& benchmarkConfigurationsCollectionModel,
			BenchmarkConfigurationModel& configurationModel);
		~BenchmarkConfigurationController() override = default;
	private:
		BenchmarkConfigurationsCollectionModel& _benchmarkConfigurationsCollectionModel;
		BenchmarkConfigurationModel& _configurationModel;
	};
}

#endif //APPS_QT_CONTROLLERS_BENCHMARKCONFIGURATIONCONTROLLER_HPP
