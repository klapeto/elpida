//
// Created by klapeto on 21/10/2023.
//

#include "FullBenchmarkResultModel.hpp"


namespace Elpida::Application
{
	FullBenchmarkResultModel::FullBenchmarkResultModel(std::vector<BenchmarkResultModel>&& benchmarkResults,
			double totalScore,
			double singleCoreScore,
			double multiCoreScore,
			double memoryScore)
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

	double FullBenchmarkResultModel::GetSingleCoreScore() const
	{
		return _singleCoreScore;
	}

	double FullBenchmarkResultModel::GetMultiCoreScore() const
	{
		return _multiCoreScore;
	}

	double FullBenchmarkResultModel::GetMemoryScore() const
	{
		return _memoryScore;
	}

	double FullBenchmarkResultModel::GetTotalScore() const
	{
		return _totalScore;
	}
} // Elpida
// Application