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

#include "Elpida/Engine/Benchmark/BenchmarkTaskInstance.hpp"

#include "Elpida/Engine/Task/Task.hpp"

namespace Elpida
{

	BenchmarkTaskInstance::BenchmarkTaskInstance(Task& task, TaskBuilder& taskBuilder)
		: _task(&task), _taskBuilder(&taskBuilder)
	{
	}

	BenchmarkTaskInstance::~BenchmarkTaskInstance()
	{
		delete _task;
	}

	BenchmarkTaskInstance& BenchmarkTaskInstance::operator=(BenchmarkTaskInstance&& other) noexcept
	{
		_task = other._task;
		other._task = nullptr;
		_taskBuilder = other._taskBuilder;
		other._taskBuilder = nullptr;
		return *this;
	}

	BenchmarkTaskInstance::BenchmarkTaskInstance(BenchmarkTaskInstance&& other) noexcept
	{
		_task = other._task;
		other._task = nullptr;
		_taskBuilder = other._taskBuilder;
		other._taskBuilder = nullptr;
	}
}