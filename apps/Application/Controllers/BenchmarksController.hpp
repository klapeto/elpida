//
// Created by klapeto on 16/4/2023.
//

#ifndef ELPIDA_BENCHMARKSCONTROLLER_HPP_
#define ELPIDA_BENCHMARKSCONTROLLER_HPP_

#include "Controller.hpp"

namespace Elpida::Application
{
	class BenchmarksModel;
	class BenchmarkModel;
	class BenchmarkConfigurationModel;

	class BenchmarksController : public Controller<BenchmarksModel>
	{
	 public:

		void SetCurrentBenchmark(const BenchmarkModel* currentBenchmark);
		void SetUploadResults(bool uploadResults);
		void SetOpenResultAfterUpload(bool openResult);
		void SetIterationsToRun(int iterations);

		explicit BenchmarksController(BenchmarksModel& model, BenchmarkConfigurationModel& configurationModel);
		~BenchmarksController() = default;
	 private:
		BenchmarkConfigurationModel& _configurationModel;
	};

} // Application

#endif //ELPIDA_BENCHMARKSCONTROLLER_HPP_
