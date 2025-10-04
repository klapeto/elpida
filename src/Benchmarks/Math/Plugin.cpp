/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
#include "NBodyBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<CalculateFFTInPlaceBenchmark>());
	vec.push_back(std::make_unique<DynamicMatrixMultiplicationBenchmark>());
	vec.push_back(std::make_unique<MatrixMultiplication32Benchmark>());
	vec.push_back(std::make_unique<MatrixInverse4Benchmark>());
	vec.push_back(std::make_unique<NBodyBenchmark>());

	return std::make_unique<BenchmarkGroup>("Math benchmarks", std::move(vec));
}