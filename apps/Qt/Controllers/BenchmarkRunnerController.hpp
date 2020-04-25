//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CONTROLLERS_BENCHMARKRUNNERCONTROLLER_HPP
#define APPS_QT_CONTROLLERS_BENCHMARKRUNNERCONTROLLER_HPP


#include <Elpida/Engine/Runner/BenchmarkRunner.hpp>
#include <Elpida/Engine/Runner/EventArgs/TaskEventArgs.hpp>
#include <Elpida/Engine/Runner/EventArgs/BenchmarkEventArgs.hpp>
#include <Elpida/OffThreadExecutor.hpp>
#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida {

	class BenchmarkResultsModel;
	class BenchmarkRunnerModel;
	class BenchmarkConfigurationsCollectionModel;
	class Mediator;

	class BenchmarkRunnerController: public CommandHandler
	{
	public:
		void handle(StartBenchmarkingCommand &command) override;
		void handle(StopBenchmarkingCommand &command) override;

		BenchmarkRunnerController(Mediator& mediator,
			BenchmarkResultsModel& benchmarkResultsModel,
			BenchmarkRunnerModel& runnerModel,
			BenchmarkConfigurationsCollectionModel& configurationsModel);
	private:
		OffThreadExecutor _taskRunnerThread;
		BenchmarkRunner _runner;
		BenchmarkResultsModel& _benchmarkResultsModel;
		BenchmarkRunnerModel& _runnerModel;
		BenchmarkConfigurationsCollectionModel& _configurationsModel;
		Mediator& _mediator;

		void onTaskBatchStarted(const BenchmarkEventArgs& ev);
		void onTaskStarted(const TaskEventArgs& ev);
		void onTaskEnded(const TaskEventArgs& ev);
		void onTaskBatchEnded(const BenchmarkEventArgs& ev);
	};
}

#endif //APPS_QT_CONTROLLERS_BENCHMARKRUNNERCONTROLLER_HPP
