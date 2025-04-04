//
// Created by klapeto on 11/4/2023.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "PngEncodingBenchmark.hpp"
#include "SvgParseBenchmark.hpp"
#include "SvgRasterizationBenchmark.hpp"
#include "SvgRasterization2Benchmark.hpp"
#include "SvgRasterizationFromFileBenchmark.hpp"
#include "RayTracingBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.emplace_back(new PngEncodingBenchmark());
	vec.emplace_back(new SvgParseBenchmark());
	vec.emplace_back(new SvgRasterizationBenchmark());
	vec.emplace_back(new SvgRasterization2Benchmark());
	vec.emplace_back(new SvgRasterizationFromFileBenchmark());
	vec.emplace_back(new RayTracingBenchmark());

	return std::make_unique<BenchmarkGroup>("Image benchmarks", std::move(vec));
}
