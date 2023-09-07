//
// Created by klapeto on 16/4/2023.
//

#ifndef ELPIDA_BENCHMARKSCONTROLLER_HPP_
#define ELPIDA_BENCHMARKSCONTROLLER_HPP_

#include "Controller.hpp"
#include "Core/Promise.hpp"

namespace Elpida::Application
{
	class BenchmarksModel;
	class BenchmarkModel;
	class TopologyModel;
	class MessageService;
	class BenchmarkExecutionService;
	class OverheadsModel;
	class BenchmarkResultsModel;

	class BenchmarksController : public Controller<BenchmarksModel>
	{
	 public:

		void SetCurrentBenchmark(const BenchmarkModel* currentBenchmark);
		void SetUploadResults(bool uploadResults);
		void SetOpenResultAfterUpload(bool openResult);
		void SetIterationsToRun(int iterations);

		void Run();
		void StopRunning();

		explicit BenchmarksController(BenchmarksModel& model, TopologyModel& topologyModel, OverheadsModel& overheadsModel, BenchmarkResultsModel& benchmarkResultsModel, BenchmarkExecutionService& benchmarkExecutionService);
		~BenchmarksController() = default;
	 private:
		TopologyModel& _topologyModel;
		OverheadsModel& _overheadsModel;
		BenchmarkResultsModel& _benchmarkResultsModel;
		BenchmarkExecutionService& _benchmarkExecutionService;
		bool _cancelling;
	};

} // Application

#endif //ELPIDA_BENCHMARKSCONTROLLER_HPP_
