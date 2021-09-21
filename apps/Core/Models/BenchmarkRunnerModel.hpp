/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 17/4/20.
//

#ifndef APPS_QT_MODELS_BENCHMARKRUNNERMODEL_HPP
#define APPS_QT_MODELS_BENCHMARKRUNNERMODEL_HPP

#include "Models/Abstractions/Model.hpp"
#include <Elpida/Utilities/OptionalReference.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

namespace Elpida
{

	class BenchmarkRunnerModel : public Model
	{
	public:

		Event<const OptionalReference<const TaskBuilder>&> currentRunningTaskChanged;
		Event<const OptionalReference<const Benchmark>&> currentRunningBenchmarkChanged;
		Event<size_t> completedBenchmarkTasksChanged;
		Event<size_t> completedTaskIterationsChanged;
		Event<size_t> completedBenchmarksChanged;
		Event<size_t> totalBenchmarksChanged;
		Event<bool> runningChanged;

		void setCurrentRunningTask(OptionalReference<const TaskBuilder> currentRunningTask)
		{
			_currentRunningTask = currentRunningTask;
			currentRunningTaskChanged.raise(_currentRunningTask);
			onDataChanged();
		}

		void setCurrentRunningBenchmark(OptionalReference<const Benchmark> currentRunningBenchmark)
		{
			_currentRunningBenchmark = currentRunningBenchmark;
			currentRunningBenchmarkChanged.raise(_currentRunningBenchmark);
			onDataChanged();
		}

		void setBenchmarkCompletedTasks(size_t benchmarkCompletedTasks)
		{
			_benchmarkCompletedTasks = benchmarkCompletedTasks;
			completedBenchmarkTasksChanged.raise(_benchmarkCompletedTasks);
			onDataChanged();
		}

		void setTaskCompletedIterations(size_t taskCompletedIterations)
		{
			_taskCompletedIterations = taskCompletedIterations;
			completedTaskIterationsChanged.raise(_taskCompletedIterations);
			onDataChanged();
		}

		void setCompletedBenchmarks(size_t completedBenchmarks)
		{
			_completedBenchmarks = completedBenchmarks;
			completedBenchmarksChanged.raise(_completedBenchmarks);
			onDataChanged();
		}

		void setTotalBenchmarks(size_t totalBenchmarks)
		{
			_totalBenchmarks = totalBenchmarks;
			totalBenchmarksChanged.raise(_totalBenchmarks);
			onDataChanged();
		}

		void setRunning(bool running)
		{
			_running = running;
			runningChanged.raise(_running);
			onDataChanged();
		}

		size_t getBenchmarkCompletedTasks() const
		{
			return _benchmarkCompletedTasks;
		}

		size_t getTaskCompletedIterations() const
		{
			return _taskCompletedIterations;
		}

		size_t getCompletedBenchmarks() const
		{
			return _completedBenchmarks;
		}

		size_t getTotalBenchmarks() const
		{
			return _totalBenchmarks;
		}

		OptionalReference<const TaskBuilder> getCurrentRunningTask() const
		{
			return _currentRunningTask;
		}

		OptionalReference<const Benchmark> getCurrentRunningBenchmark() const
		{
			return _currentRunningBenchmark;
		}

		bool isRunning() const
		{
			return _running;
		}

		BenchmarkRunnerModel()
				: _currentRunningTask(std::nullopt),
				  _currentRunningBenchmark(std::nullopt),
				  _benchmarkCompletedTasks(0),
				  _taskCompletedIterations(0),
				  _totalBenchmarks(0),
				  _completedBenchmarks(0),
				  _running(false)
		{
		}

		~BenchmarkRunnerModel() override = default;

	private:
		OptionalReference<const TaskBuilder> _currentRunningTask;
		OptionalReference<const Benchmark> _currentRunningBenchmark;
		size_t _benchmarkCompletedTasks;
		size_t _taskCompletedIterations;
		size_t _totalBenchmarks;
		size_t _completedBenchmarks;
		bool _running;
	};
}

#endif //APPS_QT_MODELS_BENCHMARKRUNNERMODEL_HPP
