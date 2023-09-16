//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_
#define ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_

#include "Elpida/Platform/Process.hpp"

#include <string>
#include <vector>

namespace Elpida::Application
{

	class BenchmarkExecutionService final
	{
	public:
		std::string Execute(const std::string& libraryPath,
				std::size_t index,
				const std::vector<std::size_t>& affinity,
				const std::vector<std::string>& configurations,
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
