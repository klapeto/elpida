//
// Created by klapeto on 16/4/2023.
//

#ifndef ELPIDA_CUSTOMBENCHMARKSCONTROLLER_HPP_
#define ELPIDA_CUSTOMBENCHMARKSCONTROLLER_HPP_

#include "Controller.hpp"
#include "Core/Promise.hpp"

namespace Elpida::Application
{
	class CustomBenchmarkModel;
	class BenchmarkModel;
	class TopologyModel;
	class MessageService;
	class BenchmarkExecutionService;
	class TimingModel;
	class CustomBenchmarkResultsModel;

	class CustomBenchmarkController : public Controller<CustomBenchmarkModel>
	{
	 public:

		void SetCurrentBenchmark(const BenchmarkModel* currentBenchmark);

		void Run();
		void StopRunning();

		explicit CustomBenchmarkController(CustomBenchmarkModel& model,
				TopologyModel& topologyModel,
				TimingModel& overheadsModel,
				CustomBenchmarkResultsModel& benchmarkResultsModel,
				BenchmarkExecutionService& benchmarkExecutionService);
		~CustomBenchmarkController() override = default;
	 private:
		TopologyModel& _topologyModel;
		TimingModel& _overheadsModel;
		CustomBenchmarkResultsModel& _benchmarkResultsModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
		bool _cancelling;
	};

} // Application

#endif //ELPIDA_CUSTOMBENCHMARKSCONTROLLER_HPP_
