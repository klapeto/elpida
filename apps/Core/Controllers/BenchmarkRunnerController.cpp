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
#include "Core/Commands/StartBenchmarkingCommand.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Models/BenchmarkRunnerModel.hpp"
#include "Models/BenchmarkResultsModel.hpp"
#include "Models/AffinityModel.hpp"
#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/BenchmarkConfigurationsCollectionModel.hpp"
#include "Models/SelectedBenchmarksModel.hpp"

#include <Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp>
#include <Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include <Elpida/SystemInfo/TimingInfo.hpp>

namespace Elpida
{
	BenchmarkRunnerController::BenchmarkRunnerController(Mediator& mediator,
			BenchmarkResultsModel& benchmarkResultsModel,
			BenchmarkRunnerModel& runnerModel,
			BenchmarkConfigurationsCollectionModel& configurationsModel,
			const SelectedBenchmarksModel& selectedBenchmarksModel,
			const AffinityModel& affinityModel,
			const ServiceProvider& serviceProvider,
			const TimingInfo& timingInfo,
			Logger& logger)
			:
			_runner(serviceProvider),
			_benchmarkResultsModel(benchmarkResultsModel),
			_runnerModel(runnerModel),
			_configurationsModel(configurationsModel),
			_selectedBenchmarksModel(selectedBenchmarksModel),
			_mediator(mediator),
			_affinityModel(affinityModel),
			_timingInfo(timingInfo),
			_logger(logger)
	{

		_runner.benchmarkStarted.subscribe([this](const auto& ev)
		{
			onBenchmarkStarted(ev);
		});

		_runner.benchmarkEnded.subscribe([this](const auto& ev)
		{
			onBenchmarkEnded(ev);
		});

		_runner.taskStarted.subscribe([this](const auto& ev)
		{
			onTaskStarted(ev);
		});

		_runner.taskIterationEnded.subscribe([this](const auto& ev)
		{
			onTaskIterationEnded(ev);
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
			if (!_selectedBenchmarksModel.empty())
			{
				auto& affinity = _affinityModel.getCurrentAffinity();
				if (!affinity.getProcessorNodes().empty())
				{
					std::vector<BenchmarkRunRequest> benchmarkRunRequests;
					const auto times = command.getTimes();
					benchmarkRunRequests.reserve(_selectedBenchmarksModel.size() * times);
					for (size_t i = 0; i < times; i++)
					{
						for (auto& pair : _selectedBenchmarksModel)
						{
							benchmarkRunRequests
								.emplace_back(pair.second.get(), _configurationsModel.getConst(pair.second.get().getUuid()));
						}
					}
					_taskRunnerThread.run([this, aff(affinity), benches(benchmarkRunRequests)]()
					{
						try
						{
							_runnerModel.transactional<BenchmarkRunnerModel>([&benches](BenchmarkRunnerModel& model)
							{
								model.setRunning(true);
								model.setTotalBenchmarks(benches.size());
							});
							auto results = _runner.runBenchmarks(benches, aff);
							_benchmarkResultsModel
									.transactional<BenchmarkResultsModel>([&results, this](BenchmarkResultsModel& model)
									{
										for (auto& result: results)
										{
											model.add(result);
										}
									});
						}
						catch (const std::exception& ex)
						{
							_logger.log(LogType::Error, "Error occurred while executing benchmarks", ex);
							_mediator.execute(ShowMessageCommand(Vu::concatenateToString(
									"Error occurred while running task batches: ",
									ex.what()), ShowMessageCommand::Type::Error));
						}

						_runnerModel.transactional<BenchmarkRunnerModel>([](BenchmarkRunnerModel& model)
						{
							model.setTotalBenchmarks(0);
							model.setCompletedBenchmarks(0);
							model.setRunning(false);
						});

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

	void BenchmarkRunnerController::onBenchmarkStarted(const BenchmarkEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([&ev](BenchmarkRunnerModel& model)
		{
			model.setCurrentRunningBenchmark(ev.getBenchmark());
			model.setBenchmarkCompletedTasks(0);
		});
	}

	void BenchmarkRunnerController::onTaskStarted(const TaskEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([&ev](BenchmarkRunnerModel& model)
		{
			model.setCurrentRunningTask(ev.getTaskBuilder());
			model.setTaskCompletedIterations(0);
		});
	}

	void BenchmarkRunnerController::onTaskEnded(const TaskEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([&ev](BenchmarkRunnerModel& model)
		{
			model.setCurrentRunningTask(std::nullopt);
			model.setTaskCompletedIterations(0);
			model.setBenchmarkCompletedTasks(model.getBenchmarkCompletedTasks() + 1);
		});
	}

	void BenchmarkRunnerController::onBenchmarkEnded(const BenchmarkEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([](BenchmarkRunnerModel& model)
		{
			model.setCurrentRunningBenchmark(std::nullopt);
			model.setBenchmarkCompletedTasks(0);
			model.setCompletedBenchmarks(model.getCompletedBenchmarks() + 1);
		});
	}

	void BenchmarkRunnerController::onTaskIterationEnded(const TaskEventArgs& ev)
	{
		_runnerModel.transactional<BenchmarkRunnerModel>([&](BenchmarkRunnerModel& model)
		{
			model.setTaskCompletedIterations(ev.getIteration());
		});
	}
}