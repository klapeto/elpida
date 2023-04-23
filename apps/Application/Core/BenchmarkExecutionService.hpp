//
// Created by klapeto on 17/4/2023.
//

#ifndef ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_
#define ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_

#include <string>
#include <tuple>
#include <vector>

namespace Elpida::Application
{

	class BenchmarkExecutionService
	{
	 public:
		std::string Execute(const std::string& libraryPath,
			std::size_t index,
			const std::vector<std::size_t>& affinity,
			const std::vector<std::string>& configurations,
			double nowOverheadNanoseconds,
			double loopOverheadNanoseconds,
			double virtualCallOverheadNanoseconds);
	 private:
	};

} // Application

#endif //ELPIDA_BENCHMARKEXECUTIONSERVICE_HPP_
