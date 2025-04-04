//
// Created by klapeto on 7/4/2024.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ModuleExports.hpp"

#include "ZlibCompressionBenchmark.hpp"
#include "ZlibDecompressionBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<ZlibCompressionBenchmark>());
	vec.push_back(std::make_unique<ZlibDecompressionBenchmark>());

	return std::make_unique<BenchmarkGroup>("Compression benchmarks", std::move(vec));
}