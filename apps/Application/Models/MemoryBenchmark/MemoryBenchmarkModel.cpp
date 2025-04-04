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