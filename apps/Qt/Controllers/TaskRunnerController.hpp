//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP


#include <Elpida/Engine/Runner/BenchmarkRunner.hpp>
#include <Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp>
#include <Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp>
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
		BenchmarkRunner _runner;
		TaskRunResultsModel& _runResultsModel;
		TaskRunnerModel& _runnerModel;
		Mediator& _mediator;

		void onTaskBatchStarted(const BenchmarkEventArgs& ev);
		void onTaskStarted(const TaskEventArgs& ev);
		void onTaskEnded(const TaskEventArgs& ev);
		void onTaskBatchEnded(const BenchmarkEventArgs& ev);
	};
}

#endif //APPS_QT_CONTROLLERS_TASKRUNNERCONTROLLER_HPP
