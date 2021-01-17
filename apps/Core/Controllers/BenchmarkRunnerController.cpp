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


#include "BenchmarkRunnerController.hpp"

#include "Core/Commands/GetBenchmarksToRunCommand.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Models/BenchmarkRunnerModel.hpp"
#include "Models/BenchmarkResultsModel.hpp"
#include "Models/AffinityModel.hpp"
#include "Models/BenchmarkConfigurationsCollectionModel.hpp"

#include <Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp>
#include <Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>

namespace Elpida
{
	BenchmarkRunnerController::BenchmarkRunnerController(Mediator& mediator,
		BenchmarkResultsModel& benchmarkResultsModel,
		BenchmarkRunnerModel& runnerModel,
		BenchmarkConfigurationsCollectionModel& configurationsModel,
		const AffinityModel& affinityModel,
		Logger& logger)
		: _benchmarkResultsModel(benchmarkResultsModel), _runnerModel(runnerModel),
		  _configurationsModel(configurationsModel), _mediator(mediator), _affinityModel(affinityModel), _logger(logger)
	{

		_runner.benchmarkStarted.subscribe([this](const auto& ev)
		{
			onTaskBatchStarted(ev);
		});

		_runner.benchmarkEnded.subscribe([this](const auto& ev)
		{
			onTaskBatchEnded(ev);
		});

		_runner.taskStarted.subscribe([this](const auto& ev)
		{
			onTaskStarted(ev);
		});

		_runner.taskEnded.subscribe([this](const auto& ev)
		{
			onTaskEnded(ev);
		});
	}

	void BenchmarkRunnerController::handle(StartBenchmarkingCommand& command)
	{
		if (!_taskRunnerThread.isRunning())
		{
			GetBenchmarksToRunCommand getSelectedCmd;
			_mediator.execute(getSelectedCmd);
			auto& benchmarks = getSelectedCmd.getBenchmarks();
			if (!benchmarks.empty())
			{
				auto& affinity = _affinityModel.getCurrentAffinity();
				if (!affinity.getProcessorNodes().empty())
				{
					std::vector<BenchmarkRunRequest> benchmarkRunRequests;
					benchmarkRunRequests.reserve(benchmarks.size());
					for (auto& bench: benchmarks)
					{
						benchmarkRunRequests
							.emplace_back(bench, _configurationsModel.getConst(bench.get().getId()));
					}
					_taskRunnerThread.run([this, aff(affinity), benches(benchmarkRunRequests)]()
					{
						try
						{
							_runnerModel.transactional<BenchmarkRunnerModel>([&benches](BenchmarkRunnerModel& model)
							{
								model.setRunning(true);
								model.setSessionTotalBenchmarksCount(benches.size());
								model.setBenchmarkCompletedTasksCount(0);
								model.setSessionCompletedBenchmarksCount(0);
							});
							auto results = _runner.runBenchmarks(benches, aff);
							_runnerModel.setRunning(false);
							_benchmarkResultsModel
								.transactional<BenchmarkResultsModel>([&results, this](BenchmarkResultsModel& model)
								{
									for (auto& result: results)
									{
										model.add(result);
									}
								});
							_runnerModel.transactional<BenchmarkRunnerModel>([&benches](BenchmarkRunnerModel& model)
							{
								model.setRunning(false);
								model.setSessionTotalBenchmarksCount(0);
								model.setBenchmarkCompletedTasksCount(0);
								model.setSessionCompletedBenchmarksCount(0);
							});
						}
						catch (const std::exception& ex)
						{
							_runnerModel.setRunning(false);
							_taskRunnerThread.detach();
							_logger.log(LogType::Error, "Error occurred while executing benchmarks", ex);
							_mediator.execute(ShowMessageCommand(Vu::concatenateToString(
								"Error occurred while running task batches: ",
								ex.what()), ShowMessageCommand::Type::Error));
						}
						_taskRunnerThread.detach();
					});
				}
				else
				{
					throw ElpidaException("You need to select at least one processor to run task batches");
				}
			}
			else
			{
				throw ElpidaException("At least 1 task batch must be selected to run!");
			}
		}
		else
		{
			throw ElpidaException("You cannot start task batches when already running!");
		}
	}

	void BenchmarkRunnerController::handle(StopBenchmarkingCommand& command)
	{
		_runner.stopBenchmarking();
	}

	void BenchmarkRunnerController::onTaskBatchStarted(const BenchmarkEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([&ev](BenchmarkRunnerModel& model)
		{
			model.setCurrentRunningTaskBatch(ev.getBenchmark());
			model.setBenchmarkCompletedTasksCount(0);
			model.setBenchmarkTotalTasksCount(ev.getBenchmark().getTotalTasksCount());
		});
	}

	void BenchmarkRunnerController::onTaskStarted(const TaskEventArgs& ev)
	{
		_runnerModel.setCurrentRunningTaskSpecification(ev.getSpecification());
	}

	void BenchmarkRunnerController::onTaskEnded(const TaskEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([&ev](BenchmarkRunnerModel& model)
		{
			model.setCurrentRunningTaskSpecification(std::nullopt);
			model.setBenchmarkCompletedTasksCount(model.getBatchExecutedTasksCount() + 1);
		});
	}

	void BenchmarkRunnerController::onTaskBatchEnded(const BenchmarkEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([](BenchmarkRunnerModel& model)
		{
			model.setCurrentRunningTaskBatch(std::nullopt);
			model.setSessionCompletedBenchmarksCount(model.getSessionCompletedBenchmarksCount() + 1);
		});
	}
}