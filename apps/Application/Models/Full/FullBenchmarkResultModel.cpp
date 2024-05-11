//
// Created by klapeto on 21/10/2023.
//

#include "FullBenchmarkResultModel.hpp"


namespace Elpida::Application
{
	FullBenchmarkResultModel::FullBenchmarkResultModel(std::vector<BenchmarkResultModel>&& benchmarkResults,
			Score totalScore,
			Score singleCoreScore,
			Score multiCoreScore,
			Score memoryScore)
			: _benchmarkResults(std::move(benchmarkResults)),
			  _totalScore(totalScore),
			  _singleCoreScore(singleCoreScore),
			  _multiCoreScore(multiCoreScore),
			  _memoryScore(memoryScore)
	{

	}

	const std::vector<BenchmarkResultModel>& FullBenchmarkResultModel::GetBenchmarkResults() const
	{
		return _benchmarkResults;
	}

	Score FullBenchmarkResultModel::GetSingleCoreScore() const
	{
		return _singleCoreScore;
	}

	Score FullBenchmarkResultModel::GetMultiCoreScore() const
	{
		return _multiCoreScore;
	}

	Score FullBenchmarkResultModel::GetMemoryScore() const
	{
		return _memoryScore;
	}

	Score FullBenchmarkResultModel::GetTotalScore() const
	{
		return _totalScore;
	}
} // Elpida
// Application