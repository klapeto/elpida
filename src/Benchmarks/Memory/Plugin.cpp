//
// Created by klapeto on 9/3/2023.
//

#include "Elpida/BenchmarkGroup.hpp"
#include "Elpida/Config.hpp"
#include "Elpida/Vector.hpp"
#include "Elpida/UniquePtr.hpp"
#include "MemoryLatencyBenchmark.hpp"
#include "MemoryReadBandwidthBenchmark.hpp"

using namespace Elpida;

extern "C" {

ELPIDA_EXPORT ELPIDA_STDCALL int GetElpidaPluginVersion()
{
	return 1;
}

ELPIDA_EXPORT ELPIDA_STDCALL BenchmarkGroup GetBenchmarkGroup()
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.emplace_back(new MemoryReadBandwidthBenchmark());

	return BenchmarkGroup("Memory benchmarks",std::move(vec));
}
}