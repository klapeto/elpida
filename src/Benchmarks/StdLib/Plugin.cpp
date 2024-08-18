#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "MallocBenchmark.hpp"
#include "StrtodBenchmark.hpp"
#include "MemsetBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<MallocBenchmark>());
	vec.push_back(std::make_unique<StrtodBenchmark>());
	vec.push_back(std::make_unique<MemsetBenchmark>());

	return std::make_unique<BenchmarkGroup>("StdLib Benchmarks", std::move(vec));
}