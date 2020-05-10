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