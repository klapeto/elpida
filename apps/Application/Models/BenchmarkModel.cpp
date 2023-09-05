//
// Created by klapeto on 8/4/2023.
//

#include "BenchmarkModel.hpp"

#include <utility>

namespace Elpida::Application
{
	BenchmarkModel::BenchmarkModel(std::string name,
		std::string filePath,
		std::size_t index,
		std::string scoreUnit,
		std::vector<TaskModel>&& tasks,
		std::vector<BenchmarkConfigurationModel>&& configurations)
		: _name(std::move(name)), _filePath(std::move(filePath)), _scoreUnit(std::move(scoreUnit)), _tasks(std::move(tasks)),
		  _configurations(std::move(configurations)), _index(index)
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

	std::size_t BenchmarkModel::GetIndex() const
	{
		return _index;
	}

	const std::vector<TaskModel>& BenchmarkModel::GetTasks() const
	{
		return _tasks;
	}

	const std::string& BenchmarkModel::GetScoreUnit() const
	{
		return _scoreUnit;
	}
} // Application