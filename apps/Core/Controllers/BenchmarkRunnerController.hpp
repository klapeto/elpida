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
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_BENCHMARKRUNNERCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_BENCHMARKRUNNERCONTROLLER_HPP

#include <Elpida/Engine/Runner/BenchmarkRunner.hpp>
#include "Elpida/Utilities/OffThreadExecutor.hpp"
#include <Elpida/Utilities/Reference.hpp>
#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida
{
	class BenchmarkResultsModel;
	class BenchmarkRunnerModel;
	class Benchmark;
	class BenchmarkConfigurationsCollectionModel;
	class SelectedBenchmarksModel;
	class AffinityModel;
	class Mediator;
	class TimingInfo;
	class Logger;

	class BenchmarkRunnerController : public CommandHandler
	{
	public:
		void handle(StartBenchmarkingCommand& command) override;
		void handle(StopBenchmarkingCommand& command) override;

		BenchmarkRunnerController(Mediator& mediator,
			BenchmarkResultsModel& benchmarkResultsModel,
			BenchmarkRunnerModel& runnerModel,
			BenchmarkConfigurationsCollectionModel& configurationsModel,
			const SelectedBenchmarksModel& selectedBenchmarksModel,
			const AffinityModel& affinityModel,
			const ServiceProvider& serviceProvider,
			const TimingInfo& timingInfo,
			Logger& logger);

		~BenchmarkRunnerController() override = default;
	private:
		OffThreadExecutor _taskRunnerThread;
		BenchmarkRunner _runner;
		BenchmarkResultsModel& _benchmarkResultsModel;
		BenchmarkRunnerModel& _runnerModel;
		BenchmarkConfigurationsCollectionModel& _configurationsModel;
		const SelectedBenchmarksModel& _selectedBenchmarksModel;
		Mediator& _mediator;
		const AffinityModel& _affinityModel;
		const TimingInfo& _timingInfo;
		Logger& _logger;

		void onBenchmarkStarted(const BenchmarkEventArgs& ev);
		void onTaskStarted(const TaskEventArgs& ev);
		void onTaskIterationEnded(const TaskEventArgs& ev);
		void onTaskEnded(const TaskEventArgs& ev);
		void onBenchmarkEnded(const BenchmarkEventArgs& ev);
	};
}

#endif //APPS_QT_CONTROLLERS_BENCHMARKRUNNERCONTROLLER_HPP
