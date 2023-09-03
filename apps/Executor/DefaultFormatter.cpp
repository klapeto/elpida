//
// Created by klapeto on 12/3/2023.
//

#include "DefaultFormatter.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include "Elpida/Core/BenchmarkInfo.hpp"

#include <sstream>

namespace Elpida
{
	std::string TranslateResult(const BenchmarkResult& result, const BenchmarkInfo& benchmarkInfo)
	{
		return ValueUtilities::GetValueScaleStringSI(result.GetScore()) + benchmarkInfo.GetScoreUnit();
	}

	String DefaultFormatter::ConvertToString(const BenchmarkResult& result, const Benchmark& benchmark) const
	{
		std::ostringstream accumulator;

		auto& benchmarkInfo = result.GetBenchmarkInfo();
		accumulator << "Score: " << TranslateResult(result, benchmarkInfo) << std::endl;

		auto& taskResults = result.GetTaskResults();
		auto taskInfos = benchmark.GetInfo().GetTaskInfos();
		for (std::size_t i = 0; i< taskResults.size(); ++i){
			auto& taskInfo = taskInfos[i];
			auto& taskResult = taskResults[i];
			if (taskInfo.GetScoreType() == Elpida::ScoreType::Throughput)
			{
				accumulator
					<< taskInfo.GetName()
					<< ": "
					<< ValueUtilities::GetValueScaleStringSI(
						(double)taskResult.GetInputSize() / taskResult.GetDuration().count())
					<< taskInfo.GetScoreUnit()
					<< "/s"
					<< std::endl;
			}
			else
			{
				accumulator
					<< taskInfo.GetName()
					<< ": "
					<< ValueUtilities::GetValueScaleStringSI(taskResult.GetDuration().count())
					<< "s"
					<< std::endl;
			}
		}
		return accumulator.str();
	}
} // Elpida