//
// Created by klapeto on 11/4/2023.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/UniquePtr.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "CalculateFFTInPlaceBenchmark.hpp"
#include "DynamicMatrixMultiplicationBenchmark.hpp"
#include "MatrixMultiplication32Benchmark.hpp"
#include "MatrixInverse4Benchmark.hpp"


using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<CalculateFFTInPlaceBenchmark>());
	vec.push_back(std::make_unique<DynamicMatrixMultiplicationBenchmark>());
	vec.push_back(std::make_unique<MatrixMultiplication32Benchmark>());
	vec.push_back(std::make_unique<MatrixInverse4Benchmark>());

	return std::make_unique<BenchmarkGroup>("Math benchmarks", std::move(vec));
}