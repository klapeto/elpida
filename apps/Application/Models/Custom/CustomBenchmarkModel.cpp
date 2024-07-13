//
// Created by klapeto on 8/4/2023.
//

#include "CustomBenchmarkModel.hpp"

namespace Elpida::Application
{
	CustomBenchmarkModel::CustomBenchmarkModel(const std::vector<BenchmarkGroupModel>& benchmarkGroups)
		: _benchmarkGroups(benchmarkGroups), _selectedBenchmark(nullptr)
	{
	}

	const std::vector<BenchmarkGroupModel>& CustomBenchmarkModel::GetBenchmarkGroups() const
	{
		return _benchmarkGroups;
	}

	void CustomBenchmarkModel::SetSelectedBenchmark(const BenchmarkModel* benchmark)
	{
		if (_selectedBenchmark != benchmark)
		{
			_selectedBenchmark = benchmark;
			OnDataChanged();
		}
	}

	const BenchmarkModel* CustomBenchmarkModel::GetSelectedBenchmark() const
	{
		return _selectedBenchmark;
	}
} // Application