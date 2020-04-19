/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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
 * Runner.cpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/Runner.hpp"

#include <chrono>

#include "Elpida/Config.hpp"
#include "Elpida/Task.hpp"
#include "Elpida/TaskBatch.hpp"
#include "Elpida/TaskMetrics.hpp"
#include "Elpida/Timer.hpp"

#ifdef ELPIDA_LINUX
#include <sys/resource.h>
#else
#include <Windows.h>
#include <iostream>
#endif

namespace Elpida
{

	Runner::Runner()
		: _mustStop(false)
	{

	}

	TaskMetrics Runner::runTask(Task& task)
	{
		task.prepare();
		task.applyAffinity();
		auto start = Timer::now();
		task.run();
		auto end = Timer::now();

		task.finalize();

		return TaskMetrics(std::chrono::duration_cast<std::chrono::nanoseconds>(end - start));
	}

	void Runner::setProcessPriority(ProcessPriority priority)
	{
#ifdef ELPIDA_LINUX
		switch (priority)
		{
		case ProcessPriority::High:
			setpriority(PRIO_PROCESS, 0, PRIO_MIN);
			break;
		default:
			setpriority(PRIO_PROCESS, 0, 0);
			break;
		}
#else
		if(!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
		{
			std::cout << "Warning! Failed to set process priority: " << GetLastError() << std::endl;
		}
#endif
	}
	std::vector<TaskBatchRunResult> Runner::executeTasks(const std::vector<const TaskBatch*>& taskBatches,
		TaskAffinity affinity)
	{
		std::vector<TaskBatchRunResult> results;

		_mustStop = false;
		for (auto taskBatch : taskBatches)
		{
			if (_mustStop) break;
			if (taskBatch != nullptr)
			{
				auto batchResult = TaskBatchRunResult(*taskBatch);

				taskBatch->prepare();
				taskBatch->onBeforeExecution();

				const auto& tasks = taskBatch->getTasks();
				raiseTaskBatchStarted(taskBatch);

				for (auto task : tasks)
				{
					if (_mustStop) break;
					task->clearResults();
					raiseTaskStart(task);
					task->setAffinity(affinity);

					TaskMetrics metrics = runTask(*task);

					if (task->isToBeMeasured())
					{
						task->calculateResults(metrics);
						const auto& taskResults = task->getLastRunResults();
						for (const auto result : taskResults)
						{
							batchResult.addResult(*task, TaskThroughput(*result, metrics));
						}
					}

					raiseTaskEnd(task);
				}
				taskBatch->onAfterExecution();
				taskBatch->finalize();

				raiseTaskBatchEnd(taskBatch, batchResult);

				results.push_back(std::move(batchResult));
			}
		}
		return results;
	}
	void Runner::raiseTaskBatchEnd(const TaskBatch* taskBatch, const TaskBatchRunResult& batchResult)
	{
		EventArguments::BatchEnd evArgs{ *taskBatch, batchResult };
		batchEnd.raise(evArgs);
	}

	void Runner::raiseTaskEnd(const Task* task)
	{
		EventArguments::TaskEnd evArgs{ *task };
		taskEnd.raise(evArgs);
	}

	void Runner::raiseTaskStart(const Task* task)
	{
		EventArguments::TaskStart evArgs{ *task };
		taskStart.raise(evArgs);
	}

	void Runner::raiseTaskBatchStarted(const TaskBatch* taskBatch)
	{
		EventArguments::BatchStart evArgs{ *taskBatch };
		batchStart.raise(evArgs);
	}

	void Runner::stopExecuting()
	{
		_mustStop = true;
	}
} /* namespace Elpida */
