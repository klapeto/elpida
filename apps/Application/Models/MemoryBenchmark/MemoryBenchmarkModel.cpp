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
// Created by klapeto on 18/6/2024.
//

#include "MemoryBenchmarkModel.hpp"

namespace Elpida::Application
{
	bool MemoryBenchmarkModel::IsRunning() const
	{
		return _running;
	}

	Event<const std::string&>& MemoryBenchmarkModel::CurrentRunningBenchmarkChanged() const
	{
		return _currentRunningBenchmarkChanged;
	}

	Event<bool>& MemoryBenchmarkModel::RunningChanged() const
	{
		return _runningChanged;
	}

	size_t MemoryBenchmarkModel::GetTotalBenchmarks() const
	{
		return _totalBenchmarks;
	}

	MemoryBenchmarkModel::MemoryBenchmarkModel()
			:_benchmarkType(MemoryBenchmarkType::MemoryLatency), _totalBenchmarks(0), _initialBytes(1024),
			 _multiplier(4.0), _iterations(10), _running(false)
	{

	}

	void MemoryBenchmarkModel::SetRunning(bool running)
	{
		if (running != _running)
		{
			_running = running;
			_runningChanged.Raise(_running);
			OnDataChanged();
		}
	}

	void MemoryBenchmarkModel::SetTotalBenchmarks(size_t totalBenchmarks)
	{
		_totalBenchmarks = totalBenchmarks;
		OnDataChanged();
	}

	void MemoryBenchmarkModel::SetCurrentRunningBenchmark(const std::string& currentRunningBenchmark)
	{
		_currentRunningBenchmark = currentRunningBenchmark;
		_currentRunningBenchmarkChanged.Raise(_currentRunningBenchmark);
		OnDataChanged();
	}

	size_t MemoryBenchmarkModel::GetIterations() const
	{
		return _iterations;
	}

	void MemoryBenchmarkModel::SetIterations(size_t iterations)
	{
		if (_iterations == iterations) return;
		_iterations = iterations;
		OnDataChanged();
	}

	size_t MemoryBenchmarkModel::GetInitialBytes() const
	{
		return _initialBytes;
	}

	double MemoryBenchmarkModel::GetMultiplier() const
	{
		return _multiplier;
	}

	void MemoryBenchmarkModel::SetInitialBytes(size_t initialBytes)
	{
		if (_initialBytes == initialBytes) return;
		_initialBytes = initialBytes;
		OnDataChanged();
	}

	void MemoryBenchmarkModel::SetMultiplier(double multiplier)
	{
		if (_multiplier == multiplier) return;
		_multiplier = multiplier;
		OnDataChanged();
	}

	MemoryBenchmarkType MemoryBenchmarkModel::GetBenchmarkType() const
	{
		return _benchmarkType;
	}

	void MemoryBenchmarkModel::SetBenchmarkType(MemoryBenchmarkType benchmarkType)
	{
		if (_benchmarkType == benchmarkType) return;
		if (benchmarkType > MemoryBenchmarkType::MemoryReadBandwidth) return; 	// Throw?
		_benchmarkType = benchmarkType;
		OnDataChanged();
	}
} // Application
// Elpida