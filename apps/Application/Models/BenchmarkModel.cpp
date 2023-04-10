//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarkModel.hpp"

#include <utility>

namespace Elpida::Application
{
	BenchmarkModel::BenchmarkModel(std::string name, std::string filePath, std::size_t index, std::vector<ConfigurationModel>&& configurations)
		: _name(std::move(name)), _filePath(std::move(filePath)), _configurations(std::move(configurations)), _index(index)
	{
	}

	const std::string& BenchmarkModel::GetName() const
	{
		return _name;
	}

	const std::vector<ConfigurationModel>& BenchmarkModel::GetConfigurations() const
	{
		return _configurations;
	}
	const std::string& BenchmarkModel::GetFilePath() const
	{
		return _filePath;
	}
	std::size_t BenchmarkModel::GetIndex() const
	{
		return _index;
	}
} // Application