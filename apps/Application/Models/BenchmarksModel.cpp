//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarksModel.hpp"

namespace Elpida::Application
{
	BenchmarksModel::BenchmarksModel(std::vector<BenchmarkGroupModel>&& benchmarkGroups)
		: _benchmarkGroups(std::move(benchmarkGroups)), _selectedBenchmark(nullptr), _iterationsToRun(1), _uploadResults(true), _openResult(false)
	{
	}

	const std::vector<BenchmarkGroupModel>& BenchmarksModel::GetBenchmarkGroups() const
	{
		return _benchmarkGroups;
	}

	void BenchmarksModel::SetSelectedBenchmark(const BenchmarkModel* benchmark)
	{
		if (_selectedBenchmark != benchmark)
		{
			_selectedBenchmark = benchmark;
			OnDataChanged();
		}
	}

	const BenchmarkModel* BenchmarksModel::GetSelectedBenchmark() const
	{
		return _selectedBenchmark;
	}

	void BenchmarksModel::SetIterationsToRun(int iterations)
	{
		_iterationsToRun = iterations;
		OnDataChanged();
	}

	void BenchmarksModel::SetUploadResults(bool uploadResults)
	{
		_uploadResults = uploadResults;
		OnDataChanged();
	}

	void BenchmarksModel::SetOpenResult(bool openResult)
	{
		_openResult = openResult;
		OnDataChanged();
	}

	int BenchmarksModel::GetIterationsToRun() const
	{
		return _iterationsToRun;
	}

	bool BenchmarksModel::IsUploadResults() const
	{
		return _uploadResults;
	}

	bool BenchmarksModel::IsOpenResult() const
	{
		return _openResult;
	}
} // Application