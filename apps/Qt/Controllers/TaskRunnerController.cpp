//
// Created by klapeto on 18/4/20.
//

#include <Elpida/Utilities/ValueUtilities.hpp>
#include "TaskRunnerController.hpp"
#include "Core/Commands/GetSelectedTaskBatchesCommand.hpp"
#include "Core/Commands/GetTaskAffinityCommand.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Models/TaskRunnerModel.hpp"
#include "Models/TaskRunResultsModel.hpp"

namespace Elpida
{
	TaskRunnerController::TaskRunnerController(Mediator& mediator,
		TaskRunResultsModel& runResultsModel,
		TaskRunnerModel& runnerModel)
		: _runResultsModel(runResultsModel), _runnerModel(runnerModel), _mediator(mediator)
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

	void TaskRunnerController::handle(StartBenchmarkingCommand& command)
	{
		if (!_taskRunnerThread.isRunning())
		{
			GetSelectedTaskBatchesCommand getSelectedCmd;
			_mediator.execute(getSelectedCmd);
			auto& taskBatches = getSelectedCmd.getTaskBatches();
			if (!taskBatches.empty())
			{
				GetTaskAffinityCommand getAffinityCmd;
				_mediator.execute(getAffinityCmd);
				auto& affinity = getAffinityCmd.getAffinity();
				if (!affinity.getProcessorNodes().empty())
				{
					_taskRunnerThread.run([this, aff(affinity), batches(taskBatches)]()
					{
						try
						{
							_runnerModel.transactional<TaskRunnerModel>([&batches](TaskRunnerModel& model)
							{
								model.setRunning(true);
								model.setSessionTotalBenchmarksCount(batches.size());
							});
							_runner.runBenchmarks(batches, aff);
							_runnerModel.setRunning(false);

						}
						catch (const std::exception& ex)
						{
							_runnerModel.setRunning(false);
							_taskRunnerThread.detach();
							_mediator.execute(ShowMessageCommand(Vu::concatenateToString("Error occurred while running task batches: ", ex.what()), ShowMessageCommand::Type::Error));
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
	void TaskRunnerController::handle(StopBenchmarkingCommand& command)
	{
		_runner.stopBenchmarking();
	}

	void TaskRunnerController::onTaskBatchStarted(const BenchmarkEventArgs& ev)
	{
		_runnerModel.transactional<TaskRunnerModel>([&ev](TaskRunnerModel& model)
		{
			model.setCurrentRunningTaskBatch(&ev.getBenchmark());
			model.setBatchExecutedTasksCount(0);
			model.setBenchmarkTotalTasksCount(ev.batch.getTasks().size());
		});
	}

	void TaskRunnerController::onTaskStarted(const TaskEventArgs& ev)
	{
		_runnerModel.setCurrentRunningTask(&ev.task);
	}

	void TaskRunnerController::onTaskEnded(const TaskEventArgs& ev)
	{
		_runnerModel.transactional<TaskRunnerModel>([&ev](TaskRunnerModel& model)
		{
			model.setCurrentRunningTask(nullptr);
			model.setBatchExecutedTasksCount(model.getBatchExecutedTasksCount() + 1);
		});
	}
	void TaskRunnerController::onTaskBatchEnded(const BenchmarkEventArgs& ev)
	{
		_runnerModel.transactional<TaskRunnerModel>([](TaskRunnerModel& model)
		{
			model.setCurrentRunningTaskBatch(nullptr);
			model.setSessionCompletedBenchmarksCount(model.getSessionCompletedBenchmarksCount() + 1);
		});

		_runResultsModel.add(ev.results);
	}
}