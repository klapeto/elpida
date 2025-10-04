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
