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

/*
 * TaskThread.hpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_ENGINE_TASK_TASKTHREAD_HPP_
#define ELPIDA_ENGINE_TASK_TASKTHREAD_HPP_

#include <thread>
#include <condition_variable>
#include <vector>
#include "Elpida/Utilities/Duration.hpp"
#include "Elpida/Engine/Result/TaskMetrics.hpp"
#include "TaskDataDto.hpp"

namespace Elpida
{
	class Task;
	class TimingInfo;
	class ExecutionParameters;

	class TaskThread final
	{
	public:
		void start();
		void join();

		const TaskMetrics& getMetrics() const
		{
			return _taskMetrics;
		}

		TaskDataDto& getTaskData()
		{
			return _taskData;
		}

		TaskThread(std::unique_ptr<Task> taskToRun,
			TaskDataDto input,
			std::condition_variable& waitNotifier,
			std::mutex& mutex,
			const TimingInfo& timingInfo,
			const ExecutionParameters& executionParameters,
			const bool& shouldWake,
			unsigned int affinity);
		~TaskThread();

		TaskThread(TaskThread&& other) noexcept;
	private:
		std::thread _runnerThread;
		std::unique_ptr<Task> _taskToRun;
		std::condition_variable& _waitNotifier;
		std::mutex& _mutex;
		TaskDataDto _taskData;
		TaskMetrics _taskMetrics = TaskMetrics(Duration::max(), 0.0, 0);
		const TimingInfo& _timingInfo;
		const ExecutionParameters& _executionParameters;

		std::string _exceptionMessage;

		const bool& _shouldWake;
		unsigned int _affinity;

		void runTask();
	};

} /* namespace Elpida */

#endif /* ELPIDA_ENGINE_TASK_TASKTHREAD_HPP_ */
