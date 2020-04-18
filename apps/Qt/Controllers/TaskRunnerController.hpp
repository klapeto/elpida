//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP


#include <Elpida/Runner.hpp>
#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida {

	class TaskRunResultsModel;
	class TaskRunnerModel;
	class Mediator;

	class TaskRunnerController: public CommandHandler
	{
	public:
		void handle(StartBenchmarkingCommand &command) override;
		void handle(StopBenchmarkingCommand &command) override;

		TaskRunnerController(Mediator& mediator,
			TaskRunResultsModel& runResultsModel,
			TaskRunnerModel& runnerModel);
	private:
		Runner _runner;
		TaskRunResultsModel& _runResultsModel;
		TaskRunnerModel& _runnerModel;
		Mediator& _mediator;
	};
}

#endif //APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
