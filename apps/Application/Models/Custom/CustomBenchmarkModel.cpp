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