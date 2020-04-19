//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP


#include <Elpida/Runner.hpp>
#include <Elpida/OffThreadExecutor.hpp>
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
		OffThreadExecutor _taskRunnerThread;
		Runner _runner;
		TaskRunResultsModel& _runResultsModel;
		TaskRunnerModel& _runnerModel;
		Mediator& _mediator;

		void onTaskBatchStarted(const Runner::EventArguments::BatchStart& ev);
		void onTaskStarted(const Runner::EventArguments::TaskStart& ev);
		void onTaskEnded(const Runner::EventArguments::TaskEnd& ev);
		void onTaskBatchEnded(const Runner::EventArguments::BatchEnd& ev);
	};
}

#endif //APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
