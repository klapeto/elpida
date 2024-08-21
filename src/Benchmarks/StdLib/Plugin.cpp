#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "StrtodBenchmark.hpp"
#include "StrtoulBenchmark.hpp"
#include "MemsetBenchmark.hpp"
#include "MemcpyBenchmark.hpp"
#include "MemchrBenchmark.hpp"
#include "SqrtBenchmark.hpp"
#include "CeilBenchmark.hpp"
#include "FloorBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<StrtodBenchmark>());
	vec.push_back(std::make_unique<StrtoulBenchmark>());
	vec.push_back(std::make_unique<MemsetBenchmark>());
	vec.push_back(std::make_unique<MemcpyBenchmark>());
	vec.push_back(std::make_unique<MemchrBenchmark>());
	vec.push_back(std::make_unique<SqrtBenchmark>());
	vec.push_back(std::make_unique<CeilBenchmark>());
	vec.push_back(std::make_unique<FloorBenchmark>());

	return std::make_unique<BenchmarkGroup>("StdLib Benchmarks", std::move(vec));
}