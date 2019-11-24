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

#include "Config.hpp"
#include "Elpida/Task.hpp"
#include "Elpida/TaskBatch.hpp"
#include "Elpida/TaskMetrics.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "Elpida/Timer.hpp"

#if _elpida_linux
#include <sys/resource.h>
#elif _elpida_windows
#include <Windows.h>
#endif

namespace Elpida
{

	Runner::Runner()
			: _mustStop(false)
	{

	}

	Runner::~Runner()
	{
	}

	void Runner::executeTasks()
	{
		_mustStop = false;
		_lastExecutionResults.clear();
		for (auto taskBatch : _tasksBatches)
		{
			if (_mustStop) break;
			auto& batchResult = _lastExecutionResults[taskBatch->getName()];
			taskBatch->prepare();
			taskBatch->onBeforeExecution();
			const auto& tasks = taskBatch->getTasks();
			{
				EventArguments::BatchStart evArgs { taskBatch->getName(), tasks.size() };
				batchStart.raise(evArgs);
			}
			for (auto task : tasks)
			{
				if (_mustStop) break;
				task->clearResults();
				{
					EventArguments::TaskStart evArgs { task->getName() };
					taskStart.raise(evArgs);
				}
				TaskMetrics metrics = runTask(*task);
				if (task->isToBeMeasured())
				{
					task->calculateResults(metrics);

					auto& resultsToExport = batchResult[task->getName()];
					const auto& results = task->getLastRunResults();
					for (auto result : results)
					{
						resultsToExport.push_back(TaskThroughput(*result, metrics));
					}
				}
				{
					EventArguments::TaskEnd evArgs { taskBatch->getName() };
					taskEnd.raise(evArgs);
				}
			}
			taskBatch->onAfterExcecution();
			taskBatch->finalize();
			{
				EventArguments::BatchEnd evArgs { taskBatch->getName(), batchResult };
				batchEnd.raise(evArgs);
			}
		}
	}

	void Runner::addTaskBatch(const TaskBatch &batch)
	{
		_tasksBatches.push_back(&batch);
	}

	TaskMetrics Runner::runTask(Task &task)
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
#if _elpida_linux
		switch (priority)
		{
			case ProcessPriority::High:
				setpriority(PRIO_PROCESS, 0, PRIO_MIN);
				break;
			default:
				setpriority(PRIO_PROCESS, 0, 0);
				break;
		}
#elif _elpida_windows
		if(!SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS))
		{
			std::cout << "Warning! Failed to set process priority: " << GetLastError() << std::endl;
		}
#endif
	}

}/* namespace Elpida */
