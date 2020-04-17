//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP


#include <Elpida/Runner.hpp>
namespace Elpida {

	class TaskRunResultsModel;
	class TaskRunnerModel;

	class TaskRunnerController
	{
	public:
		TaskRunnerController(TaskRunResultsModel& runResultsModel, TaskRunnerModel& runnerModel);
	private:
		Runner _runner;
		TaskRunResultsModel& _runResultsModel;
		TaskRunnerModel& _runnerModel;
	};
}

#endif //APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
