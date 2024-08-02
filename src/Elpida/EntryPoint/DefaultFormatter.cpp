//
// Created by klapeto on 12/3/2023.
//

#include "Elpida/EntryPoint/DefaultFormatter.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/BenchmarkInfo.hpp"

#include <sstream>

namespace Elpida
{
	std::string TranslateResult(const BenchmarkResult& result, const BenchmarkInfo& benchmarkInfo)
	{
		return ValueUtilities::GetValueScaleStringSI(result.GetResult()) + benchmarkInfo.GetResultUnit() + (benchmarkInfo.GetResultType() == ResultType::Throughput ? "/s" : "");
	}

	String DefaultFormatter::ConvertToString(const BenchmarkResult& result, const Benchmark& benchmark) const
	{
		std::ostringstream accumulator;

		auto benchmarkInfo = benchmark.GetInfo();
		accumulator << "Score: " << TranslateResult(result, benchmarkInfo) << std::endl;
		return accumulator.str();
	}
} // Elpida