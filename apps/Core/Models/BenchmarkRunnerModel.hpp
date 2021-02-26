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

		void setCurrentRunningTask(OptionalReference<const TaskBuilder> currentRunningTask)
		{
			_currentRunningTask = currentRunningTask;
			onDataChanged();
		}

		void setCurrentRunningBenchmark(OptionalReference<const Benchmark> currentRunningTaskBatch)
		{
			_currentRunningBenchmark = currentRunningTaskBatch;
			onDataChanged();
		}

		size_t getBenchmarkCompletedTasks() const
		{
			return _benchmarkCompletedTasks;
		}

		void setBenchmarkCompletedTasks(size_t benchmarkCompletedTasks)
		{
			_benchmarkCompletedTasks = benchmarkCompletedTasks;
			onDataChanged();
		}

		size_t getTaskCompletedIterations() const
		{
			return _taskCompletedIterations;
		}

		void setTaskCompletedIterations(size_t taskCompletedIterations)
		{
			_taskCompletedIterations = taskCompletedIterations;
			onDataChanged();
		}

		OptionalReference<const TaskBuilder> getCurrentRunningTask() const
		{
			return _currentRunningTask;
		}

		OptionalReference<const Benchmark> getCurrentRunningBenchmark() const
		{
			return _currentRunningBenchmark;
		}

		BenchmarkRunnerModel()
			: _currentRunningTask(std::nullopt),
			  _currentRunningBenchmark(std::nullopt),
			  _benchmarkCompletedTasks(0),
			  _taskCompletedIterations(0)
		{
		}

		~BenchmarkRunnerModel() override = default;
	private:
		OptionalReference<const TaskBuilder> _currentRunningTask;
		OptionalReference<const Benchmark> _currentRunningBenchmark;
		size_t _benchmarkCompletedTasks;
		size_t _taskCompletedIterations;

	};
}

#endif //APPS_QT_MODELS_BENCHMARKRUNNERMODEL_HPP
