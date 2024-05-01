//
// Created by klapeto on 24/4/2023.
//

#include "JsonFormatter.hpp"

#include "JsonSerializer.hpp"

using namespace nlohmann;

namespace Elpida
{
	String JsonFormatter::ConvertToString(const BenchmarkResult& result, const Benchmark& benchmark) const
	{
		return JsonSerializer::Serialize(result).dump();
	}
} // Elpida