//
// Created by klapeto on 9/3/2023.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
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