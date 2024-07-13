//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarkModel.hpp"

#include <utility>

namespace Elpida::Application
{
	BenchmarkModel::BenchmarkModel(
			std::string name,
			std::string description,
			std::string resultUnit,
			ResultType resultType,
			std::string filePath,
			std::size_t benchmarkIndex,
			std::vector<BenchmarkConfigurationModel>&& configurations)
			: _name(std::move(name)),
			_description(std::move(description)),
			_filePath(std::move(filePath)),
			_resultUnit(std::move(resultUnit)),
			_configurations(std::move(configurations)),
			_benchmarkIndex(benchmarkIndex),
			_resultType(resultType)
	{
	}

	const std::string& BenchmarkModel::GetName() const
	{
		return _name;
	}

	const std::vector<BenchmarkConfigurationModel>& BenchmarkModel::GetConfigurations() const
	{
		return _configurations;
	}

	const std::string& BenchmarkModel::GetFilePath() const
	{
		return _filePath;
	}

	std::size_t BenchmarkModel::GetBenchmarkIndex() const
	{
		return _benchmarkIndex;
	}

	const std::string& BenchmarkModel::GetDescription() const
	{
		return _description;
	}

	const std::string& BenchmarkModel::GetResultUnit() const
	{
		return _resultUnit;
	}

	ResultType BenchmarkModel::GetResultType() const
	{
		return _resultType;
	}
} // Application