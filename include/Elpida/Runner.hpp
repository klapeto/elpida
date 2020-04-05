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
#include "Elpida/TaskThroughput.hpp"
#include "Elpida/TaskAffinity.hpp"

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
				const std::string& name;
				std::size_t numberOfTasks;
			};
			struct TaskStart
			{
				const std::string& name;
			};

			struct TaskEnd
			{
				const std::string& name;
			};
			struct BatchEnd
			{
				const std::string& name;
				const std::unordered_map<std::string, std::vector<TaskThroughput>>& results;
			};
		};

		Event<const EventArguments::BatchStart&> batchStart;
		Event<const EventArguments::TaskStart&> taskStart;
		Event<const EventArguments::TaskEnd&> taskEnd;
		Event<const EventArguments::BatchEnd&> batchEnd;

		static void setProcessPriority(ProcessPriority priority);

		void executeTasks();
		void addTaskBatch(const TaskBatch& batch);
		void stop()
		{
			_mustStop = true;
		}

		const std::unordered_map<std::string,
								 std::unordered_map<std::string,
													std::vector<TaskThroughput>>>& getLastExecutionResults() const
		{
			return _lastExecutionResults;
		}

		void clearTaskBatches()
		{
			_tasksBatches.clear();
		}

		void setTaskAffinity(const TaskAffinity& taskAffinity)
		{
			_taskAffinity = taskAffinity;
		}

		Runner();
		virtual ~Runner();

		Runner(Runner&&) = default;
		Runner(const Runner&) = default;
		Runner& operator=(Runner&&) = default;
		Runner& operator=(const Runner&) = default;
	private:
		std::unordered_map<std::string, std::unordered_map<std::string, std::vector<TaskThroughput>>>
			_lastExecutionResults;
		std::vector<const TaskBatch*> _tasksBatches;
		TaskAffinity _taskAffinity;
		bool _mustStop;
		TaskMetrics runTask(Task& task);
	};

} /* namespace Elpida */

#endif /* ELPIDA_RUNNER_HPP_ */
