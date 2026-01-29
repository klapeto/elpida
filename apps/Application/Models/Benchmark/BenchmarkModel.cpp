/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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