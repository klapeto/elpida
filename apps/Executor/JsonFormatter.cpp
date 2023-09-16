//
// Created by klapeto on 24/4/2023.
//

#include "JsonFormatter.hpp"

#include "Elpida/Core/Duration.hpp"
#include "json.hpp"

using namespace nlohmann;

namespace Elpida
{
	String JsonFormatter::ConvertToString(const BenchmarkResult& result, const Benchmark& benchmark) const
	{
		json rootJ;

		rootJ["score"] = result.GetScore();

		auto arrayJ = json::array();

		auto& taskResults = result.GetTaskResults();
		for (const auto& taskResult: taskResults)
		{
			json resultJ;

			resultJ["durationNanoseconds"] = std::chrono::duration_cast<NanoSeconds>(
					taskResult.GetDuration()).count();
			resultJ["dataSize"] = taskResult.GetDataSize();

			arrayJ.push_back(std::move(resultJ));
		}
		rootJ["taskResults"] = std::move(arrayJ);
		return rootJ.dump();
	}
} // Elpida