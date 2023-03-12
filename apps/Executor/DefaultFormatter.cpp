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

	String DefaultFormatter::ConvertToString(const BenchmarkResult& result) const
	{
		std::ostringstream accumulator;

		auto& benchmarkInfo = result.GetBenchmarkInfo();
		accumulator << "Result: " << TranslateResult(result, benchmarkInfo) << std::endl;

		auto& taskResults = result.GetTaskResults();
		for (const auto& taskResult: taskResults)
		{
			auto& taskInfo = taskResult.GetTaskInfo();
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