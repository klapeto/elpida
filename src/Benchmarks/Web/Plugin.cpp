//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

//
// Created by klapeto on 7/4/2024.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/ModuleExports.hpp"
#include "XmlParseBenchmark.hpp"
#include "JsonParseBenchmark.hpp"
#include "Base64EncodeBenchmark.hpp"
#include "Base64DecodeBenchmark.hpp"
#include "RegexBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<XmlParseBenchmark>());
	vec.push_back(std::make_unique<JsonParseBenchmark>());
	vec.push_back(std::make_unique<Base64EncodeBenchmark>());
	vec.push_back(std::make_unique<Base64DecodeBenchmark>());
	vec.push_back(std::make_unique<RegexBenchmark>());

	return std::make_unique<BenchmarkGroup>("Web benchmarks", std::move(vec));
}

