//
// Created by klapeto on 7/4/2024.
//

#include "Elpida/Core/BenchmarkGroup.hpp"
#include "Elpida/Core/Config.hpp"
#include "Elpida/Core/Vector.hpp"
#include "Elpida/Core/BenchmarkModule.hpp"
#include "XmlParseBenchmark.hpp"
#include "JsonParseBenchmark.hpp"

using namespace Elpida;

ELPIDA_CREATE_BENCHMARK_GROUP_DECL
{
	Vector<UniquePtr<Benchmark>> vec;

	vec.push_back(std::make_unique<XmlParseBenchmark>());
	vec.push_back(std::make_unique<JsonParseBenchmark>());

	return new BenchmarkGroup("Web benchmarks", std::move(vec));
}

ELPIDA_DESTROY_BENCHMARK_GROUP_DECL
{
	delete group;
}
