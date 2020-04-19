//
// Created by klapeto on 18/4/20.
//

#include "TaskRunnerController.hpp"
#include "Core/Commands/GetSelectedTaskBatchesCommand.hpp"
#include "Core/Commands/GetTaskAffinityCommand.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Models/TaskRunnerModel.hpp"
#include "Models/TaskRunResultsModel.hpp"

#include <Elpida/TaskBatch.hpp>

namespace Elpida
{
	TaskRunnerController::TaskRunnerController(Mediator& mediator,
		TaskRunResultsModel& runResultsModel,
		TaskRunnerModel& runnerModel)
		: _runResultsModel(runResultsModel), _runnerModel(runnerModel), _mediator(mediator)
	{

		_runner.batchStart.subscribe([this](const Runner::EventArguments::BatchStart& ev)
		{
			onTaskBatchStarted(ev);
		});

		_runner.batchEnd.subscribe([this](const Runner::EventArguments::BatchEnd& ev)
		{
			onTaskBatchEnded(ev);
		});

		_runner.taskStart.subscribe([this](const Runner::EventArguments::TaskStart& ev)
		{
			onTaskStarted(ev);
		});

		_runner.taskEnd.subscribe([this](const Runner::EventArguments::TaskEnd& ev)
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
						_runnerModel.transactional<TaskRunnerModel>([&batches](TaskRunnerModel& model)
						{
							model.setRunning(true);
							model.setSessionTotalTaskBatchesCount(batches.size());
						});
						_runner.executeTasks(batches, aff);
						_runnerModel.setRunning(false);
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
		_runner.stopExecuting();
	}

	void TaskRunnerController::onTaskBatchStarted(const Runner::EventArguments::BatchStart& ev)
	{
		_runnerModel.transactional<TaskRunnerModel>([&ev](TaskRunnerModel& model)
		{
			model.setCurrentRunningTaskBatch(&ev.batch);
			model.setBatchExecutedTasksCount(0);
			model.setBatchTotalTasksCount(ev.batch.getTasks().size());
		});
	}

	void TaskRunnerController::onTaskStarted(const Runner::EventArguments::TaskStart& ev)
	{
		_runnerModel.setCurrentRunningTask(&ev.task);
	}

	void TaskRunnerController::onTaskEnded(const Runner::EventArguments::TaskEnd& ev)
	{
		_runnerModel.transactional<TaskRunnerModel>([&ev](TaskRunnerModel& model)
		{
			model.setCurrentRunningTask(nullptr);
			model.setBatchExecutedTasksCount(model.getBatchExecutedTasksCount() + 1);
		});
	}
	void TaskRunnerController::onTaskBatchEnded(const Runner::EventArguments::BatchEnd& ev)
	{
		_runnerModel.transactional<TaskRunnerModel>([](TaskRunnerModel& model)
		{
			model.setCurrentRunningTaskBatch(nullptr);
			model.setSessionExecutedTaskBatchesCount(model.getSessionExecutedTaskBatchesCount() + 1);
		});

		_runResultsModel.add(ev.results);
	}
}