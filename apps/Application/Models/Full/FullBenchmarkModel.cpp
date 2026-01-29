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
// Created by klapeto on 22/10/2023.
//

#include "FullBenchmarkModel.hpp"

namespace Elpida::Application
{
	FullBenchmarkModel::FullBenchmarkModel()
			: _totalBenchmarks(1), _running(false)
	{

	}

	void FullBenchmarkModel::SetRunning(bool running)
	{
		if (running != _running)
		{
			_running = running;
			_runningChanged.Raise(_running);
			OnDataChanged();
		}
	}

	void FullBenchmarkModel::SetCurrentRunningBenchmark(const std::string& benchmark)
	{
		if (_currentRunningBenchmark != benchmark)
		{
			_currentRunningBenchmark = benchmark;
			_currentRunningBenchmarkChanged.Raise(_currentRunningBenchmark);
			OnDataChanged();
		}
	}

	void FullBenchmarkModel::SetTotalBenchmarks(std::size_t totalBenchmarks)
	{
		_totalBenchmarks = totalBenchmarks;
		OnDataChanged();
	}

	const std::string& FullBenchmarkModel::GetCurrentRunningBenchmark() const
	{
		return _currentRunningBenchmark;
	}

	size_t FullBenchmarkModel::GetTotalBenchmarks() const
	{
		return _totalBenchmarks;
	}

	bool FullBenchmarkModel::IsRunning() const
	{
		return _running;
	}

	Event<bool>& FullBenchmarkModel::RunningChanged() const
	{
		return _runningChanged;
	}

	Event<const std::string&>& FullBenchmarkModel::CurrentRunningBenchmarkChanged() const
	{
		return _currentRunningBenchmarkChanged;
	}
} // Elpida
// Application