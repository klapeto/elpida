//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarksModel.hpp"

namespace Elpida::Application
{
	BenchmarksModel::BenchmarksModel(std::vector<BenchmarkGroupModel>&& benchmarkGroups)
		: _benchmarkGroups(std::move(benchmarkGroups)), _selectedBenchmark(nullptr)
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
} // Application