//
// Created by klapeto on 17/4/2023.
//

#include "BenchmarkExecutionService.hpp"

namespace Elpida::Application
{
	std::string
	BenchmarkExecutionService::Execute(const std::string& libraryPath, std::size_t index, const std::vector<int>& affinity, const std::vector<std::tuple<std::string, std::string>>& configuration, double nowOverheadNanoseconds, double loopOverheadNanoseconds, double virtualCallOverheadNanoseconds)
	{
		return std::string();
	}
} // Application