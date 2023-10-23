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

	void FullBenchmarkModel::AddResult(FullBenchmarkResultModel&& result)
	{
		_results.push_back(std::move(result));
		OnDataChanged();
	}

	const std::vector<FullBenchmarkResultModel>& FullBenchmarkModel::GetResults() const
	{
		return _results;
	}
} // Elpida
// Application