//
// Created by klapeto on 17/4/20.
//

#include "Elpida/TaskBatchRunResult.hpp"
#include "Elpida/Task.hpp"
#include "Elpida/Utilities/Uuid.hpp"

namespace Elpida
{

	TaskBatchRunResult::TaskBatchRunResult(const TaskBatch& taskBatch)
		: _id(Uuid::create()) ,_taskBatch(taskBatch)
	{

	}
	void TaskBatchRunResult::addResult(const Task& task, TaskThroughput&& throughput)
	{
		_tasksThroughputs.emplace(task.getName(), throughput);
	}
}
