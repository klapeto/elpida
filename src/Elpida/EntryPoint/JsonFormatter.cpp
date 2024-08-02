//
// Created by klapeto on 24/4/2023.
//

#include "Elpida/EntryPoint/JsonFormatter.hpp"
#include "json.hpp"

namespace Elpida
{
	String JsonFormatter::ConvertToString(const BenchmarkResult& result, const Benchmark& benchmark) const
	{
		nlohmann::json rootJ;

		rootJ["result"] = result.GetResult();

		return rootJ.dump();
	}
} // Elpida