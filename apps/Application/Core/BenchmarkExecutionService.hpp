//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_
#define ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_

#include "Elpida/Platform/Process.hpp"
#include "Models/Benchmark/BenchmarkResultModel.hpp"

#include <string>
#include <vector>

namespace Elpida::Application
{
	class BenchmarkModel;

	class BenchmarkExecutionService final
	{
	public:
		BenchmarkResultModel Execute(const BenchmarkModel& benchmarkModel,
				const std::vector<std::size_t>& affinity,
				double nowOverheadNanoseconds,
				double loopOverheadNanoseconds,
				double virtualCallOverheadNanoseconds);

		void StopCurrentExecution();

		BenchmarkExecutionService() = default;
		~BenchmarkExecutionService() = default;
	private:
		Process _currentProcess;
	};

} // Application

#endif //ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_
