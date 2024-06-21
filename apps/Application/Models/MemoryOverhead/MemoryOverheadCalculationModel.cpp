//
// Created by klapeto on 18/6/2024.
//

#include "MemoryOverheadCalculationModel.hpp"


namespace Elpida::Application
{
	bool MemoryOverheadCalculationModel::IsRunning() const
	{
		return _running;
	}

	Event<const std::string&>& MemoryOverheadCalculationModel::CurrentRunningBenchmarkChanged() const
	{
		return _currentRunningBenchmarkChanged;
	}

	Event<bool>& MemoryOverheadCalculationModel::RunningChanged() const
	{
		return _runningChanged;
	}

	size_t MemoryOverheadCalculationModel::GetTotalBenchmarks() const
	{
		return _totalBenchmarks;
	}

	MemoryOverheadCalculationModel::MemoryOverheadCalculationModel()
			:_totalBenchmarks(0), _running(false), _initialScale(0.01), _scaleMultiplier(2.0), _initialSubSamples(32), _iterations(6), _subSamplesMultiplier(1.0)
	{

	}

	void MemoryOverheadCalculationModel::SetRunning(bool running)
	{
		if (running != _running)
		{
			_running = running;
			_runningChanged.Raise(_running);
			OnDataChanged();
		}
	}

	void MemoryOverheadCalculationModel::SetTotalBenchmarks(size_t totalBenchmarks)
	{
		_totalBenchmarks = totalBenchmarks;
		OnDataChanged();
	}

	void MemoryOverheadCalculationModel::SetCurrentRunningBenchmark(const std::string& currentRunningBenchmark)
	{
		if (_currentRunningBenchmark != currentRunningBenchmark)
		{
			_currentRunningBenchmark = currentRunningBenchmark;
			_currentRunningBenchmarkChanged.Raise(_currentRunningBenchmark);
			OnDataChanged();
		}
	}

	double MemoryOverheadCalculationModel::GetSubSamplesMultiplier() const
	{
		return _subSamplesMultiplier;
	}

	void MemoryOverheadCalculationModel::SetSubSamplesMultiplier(double subSamplesMultiplier)
	{
		if (_subSamplesMultiplier == subSamplesMultiplier) return;
		_subSamplesMultiplier = subSamplesMultiplier;
		OnDataChanged();
	}

	size_t MemoryOverheadCalculationModel::GetInitialSubSamples() const
	{
		return _initialSubSamples;
	}

	void MemoryOverheadCalculationModel::SetInitialSubSamples(size_t initialSubSamples)
	{
		if (_initialSubSamples == initialSubSamples) return;
		_initialSubSamples = initialSubSamples;
		OnDataChanged();
	}

	double MemoryOverheadCalculationModel::GetScaleMultiplier() const
	{
		return _scaleMultiplier;
	}

	void MemoryOverheadCalculationModel::SetScaleMultiplier(double scaleMultiplier)
	{
		if (_scaleMultiplier == scaleMultiplier) return;
		_scaleMultiplier = scaleMultiplier;
		OnDataChanged();
	}

	double MemoryOverheadCalculationModel::GetInitialScale() const
	{
		return _initialScale;
	}

	void MemoryOverheadCalculationModel::SetInitialScale(double initialScale)
	{
		if (_initialScale == initialScale) return;
		_initialScale = initialScale;
		OnDataChanged();
	}

	size_t MemoryOverheadCalculationModel::GetIterations() const
	{
		return _iterations;
	}

	void MemoryOverheadCalculationModel::SetIterations(size_t iterations)
	{
		_iterations = iterations;
	}
} // Application
// Elpida