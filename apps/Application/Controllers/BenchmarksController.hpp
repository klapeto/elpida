//
// Created by klapeto on 16/4/2023.
//

#ifndef ELPIDA_BENCHMARKSCONTROLLER_HPP_
#define ELPIDA_BENCHMARKSCONTROLLER_HPP_

namespace Elpida::Application
{
	class BenchmarksModel;
	class BenchmarkModel;
	class BenchmarksController
	{
	 public:

		explicit BenchmarksController(BenchmarksModel& model);
		~BenchmarksController() = default;
	 private:
		BenchmarksModel& _model;
	};

} // Application

#endif //ELPIDA_BENCHMARKSCONTROLLER_HPP_
