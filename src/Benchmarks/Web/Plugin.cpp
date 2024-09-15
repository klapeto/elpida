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

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<XmlParseBenchmark>());
	vec.push_back(std::make_unique<JsonParseBenchmark>());
	vec.push_back(std::make_unique<Base64EncodeBenchmark>());
	vec.push_back(std::make_unique<Base64DecodeBenchmark>());

	return std::make_unique<BenchmarkGroup>("Web benchmarks", std::move(vec));
}

