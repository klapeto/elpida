//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarksModel.hpp"

namespace Elpida::Application
{
	BenchmarksModel::BenchmarksModel(std::vector<BenchmarkGroupModel>&& benchmarkGroups)
		: _benchmarkGroups(std::move(benchmarkGroups))
	{
	}
	const std::vector<BenchmarkGroupModel>& BenchmarksModel::GetBenchmarkGroups() const
	{
		return _benchmarkGroups;
	}
} // Application