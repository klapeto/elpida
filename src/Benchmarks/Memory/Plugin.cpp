//
// Created by klapeto on 9/3/2023.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "MemoryLatencyBenchmark.hpp"
#include "MemoryReadBandwidthBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.emplace_back(new MemoryReadBandwidthBenchmark());
	vec.emplace_back(new MemoryLatencyBenchmark());

	return std::make_unique<BenchmarkGroup>("Memory benchmarks", std::move(vec));
}
