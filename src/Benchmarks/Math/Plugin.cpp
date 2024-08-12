//
// Created by klapeto on 11/4/2023.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "CalculateFFTInPlaceBenchmark.hpp"
#include "MatrixMultiplicationBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<CalculateFFTInPlaceBenchmark>());
	vec.push_back(std::make_unique<MatrixMultiplicationBenchmark>());

	return std::make_unique<BenchmarkGroup>("Math benchmarks", std::move(vec));
}