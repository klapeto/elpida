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

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "MallocBenchmark.hpp"
#include "StrtodBenchmark.hpp"
#include "StrtoulBenchmark.hpp"
#include "MemsetBenchmark.hpp"
#include "MemchrBenchmark.hpp"
#include "SqrtBenchmark.hpp"
#include "CeilBenchmark.hpp"
#include "FloorBenchmark.hpp"
#include "RegexBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<MallocBenchmark>());
	vec.push_back(std::make_unique<StrtodBenchmark>());
	vec.push_back(std::make_unique<StrtoulBenchmark>());
	vec.push_back(std::make_unique<MemsetBenchmark>());
	vec.push_back(std::make_unique<MemchrBenchmark>());
	vec.push_back(std::make_unique<SqrtBenchmark>());
	vec.push_back(std::make_unique<CeilBenchmark>());
	vec.push_back(std::make_unique<FloorBenchmark>());
	vec.push_back(std::make_unique<RegexBenchmark>());

	return std::make_unique<BenchmarkGroup>("StdLib Benchmarks", std::move(vec));
}