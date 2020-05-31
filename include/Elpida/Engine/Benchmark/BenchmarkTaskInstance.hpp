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
// Created by klapeto on 10/5/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_BENCHMARK_BENCHMARKTASKINSTANCE_HPP
#define INCLUDE_ELPIDA_ENGINE_BENCHMARK_BENCHMARKTASKINSTANCE_HPP

namespace Elpida
{
	class Task;
	class TaskBuilder;

	class BenchmarkTaskInstance final
	{
	public:

		[[nodiscard]] Task& getTask() const
		{
			return *_task;
		}

		[[nodiscard]] const TaskBuilder& getTaskBuilder() const
		{
			return *_taskBuilder;
		}

		BenchmarkTaskInstance(Task& task, TaskBuilder& taskBuilder);
		BenchmarkTaskInstance(BenchmarkTaskInstance&& other) noexcept;
		BenchmarkTaskInstance& operator=(BenchmarkTaskInstance&& other) noexcept;
		BenchmarkTaskInstance(const BenchmarkTaskInstance&) = delete;
		BenchmarkTaskInstance& operator=(const BenchmarkTaskInstance&) = delete;
		~BenchmarkTaskInstance();
	private:
		Task* _task;
		TaskBuilder* _taskBuilder;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_BENCHMARK_BENCHMARKTASKINSTANCE_HPP
