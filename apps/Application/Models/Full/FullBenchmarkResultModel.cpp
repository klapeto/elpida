//
// Created by klapeto on 21/10/2023.
//

#include "FullBenchmarkResultModel.hpp"


namespace Elpida::Application
{
	FullBenchmarkResultModel::FullBenchmarkResultModel(std::vector<BenchmarkResultModel>&& benchmarkResults,
			Score totalScore,
			Score singleThreadScore,
			Score multiThreadScore)
			:_benchmarkResults(std::move(benchmarkResults)),
			 _totalScore(totalScore),
			 _singleThreadScore(singleThreadScore),
			 _multiThreadScore(multiThreadScore)
	{

	}

	const std::vector<BenchmarkResultModel>& FullBenchmarkResultModel::GetBenchmarkResults() const
	{
		return _benchmarkResults;
	}

	Score FullBenchmarkResultModel::GetSingleThreadScore() const
	{
		return _singleThreadScore;
	}

	Score FullBenchmarkResultModel::GetMultiThreadScore() const
	{
		return _multiThreadScore;
	}

	Score FullBenchmarkResultModel::GetTotalScore() const
	{
		return _totalScore;
	}
} // Elpida
// Application