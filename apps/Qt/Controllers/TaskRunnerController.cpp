//
// Created by klapeto on 18/4/20.
//

#include "TaskRunnerController.hpp"


namespace Elpida
{

	TaskRunnerController::TaskRunnerController(TaskRunResultsModel& runResultsModel, TaskRunnerModel& runnerModel)
		: _runResultsModel(runResultsModel), _runnerModel(runnerModel)
	{
	}
}