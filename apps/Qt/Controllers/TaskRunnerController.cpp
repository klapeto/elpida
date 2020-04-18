//
// Created by klapeto on 18/4/20.
//

#include "TaskRunnerController.hpp"
#include "Core/Commands/GetSelectedTaskBatchesCommand.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"
#include "Core/Abstractions/Mediator.hpp"

namespace Elpida
{
	TaskRunnerController::TaskRunnerController(Mediator& mediator,
		TaskRunResultsModel& runResultsModel,
		TaskRunnerModel& runnerModel)
		: _runResultsModel(runResultsModel), _runnerModel(runnerModel), _mediator(mediator)
	{
	}
	void TaskRunnerController::handle(StartBenchmarkingCommand& command)
	{
		GetSelectedTaskBatchesCommand getSelectedCmd;
		_mediator.execute(getSelectedCmd);
		auto& taskBatches = getSelectedCmd.getTaskBatches();
		if (!taskBatches.empty())
		{

		}
		else
		{
			_mediator.execute(ShowMessageCommand("At least 1 task batch must be selected to run!",
				ShowMessageCommand::Type::Error));
		}

	}
	void TaskRunnerController::handle(StopBenchmarkingCommand& command)
	{

	}
}