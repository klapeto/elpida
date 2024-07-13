//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarkGroupModel.hpp"

#include <utility>

namespace Elpida::Application
{
	BenchmarkGroupModel::BenchmarkGroupModel(std::string name, std::vector<BenchmarkModel>&& benchmarks)
		: _name(std::move(name)), _benchmarks(std::move(benchmarks))
	{
	}

	const std::string& BenchmarkGroupModel::GetName() const
	{
		return _name;
	}

	const std::vector<BenchmarkModel>& BenchmarkGroupModel::GetBenchmarks() const
	{
		return _benchmarks;
	}
} // Application