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
 * Runner.hpp
 *
 *  Created on: 8 Μαρ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_RUNNER_HPP_
#define ELPIDA_RUNNER_HPP_

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

#include "Elpida/Event.hpp"
#include "Elpida/TaskAffinity.hpp"
#include "Elpida/TaskBatchRunResult.hpp"

namespace Elpida
{

	class Task;
	class TaskBatch;

	class Runner
	{
	public:
		enum class ProcessPriority
		{
			Normal, High,
		};

		struct EventArguments
		{
			struct BatchStart
			{
				const TaskBatch& batch;
			};
			struct TaskStart
			{
				const Task& task;
			};

			struct TaskEnd
			{
				const Task& task;
			};
			struct BatchEnd
			{
				const TaskBatch& batch;
				const TaskBatchRunResult& results;
			};
		};

		Event<const EventArguments::BatchStart&> batchStart;
		Event<const EventArguments::TaskStart&> taskStart;
		Event<const EventArguments::TaskEnd&> taskEnd;
		Event<const EventArguments::BatchEnd&> batchEnd;

		static void setProcessPriority(ProcessPriority priority);

		std::vector<TaskBatchRunResult>  executeTasks(const std::vector<const TaskBatch*>& taskBatches, TaskAffinity affinity);
		void stopExecuting();

		Runner();
		virtual ~Runner() = default;
	private:
		bool _mustStop;

		static TaskMetrics runTask(Task& task);

		void raiseTaskBatchStarted(const TaskBatch* taskBatch);
		void raiseTaskStart(const Task* task);
		void raiseTaskEnd(const Task* task);
		void raiseTaskBatchEnd(const TaskBatch* taskBatch, const TaskBatchRunResult& batchResult);
	};

} /* namespace Elpida */

#endif /* ELPIDA_RUNNER_HPP_ */
