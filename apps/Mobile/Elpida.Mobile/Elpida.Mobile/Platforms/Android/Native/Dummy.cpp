//
// Created by klapeto on 19/9/26.
//

#include <Elpida/Core/ModuleExports.hpp>

ELPIDA_CREATE_BENCHMARK_GROUP_FUNC();

extern "C" std::unique_ptr<Elpida::BenchmarkGroup> DoCreateBenchmarkGroup()
{
	// Trick to force resolve all linked libraries
	return ELPIDA_CREATE_BENCHMARK_GROUP_NAME();
}