//
// Created by klapeto on 17/4/20.
//

#ifndef INCLUDE_ELPIDA_TASKBATCHRUNRESULT_HPP
#define INCLUDE_ELPIDA_TASKBATCHRUNRESULT_HPP

#include <unordered_map>
#include "Elpida/TaskThroughput.hpp"

namespace Elpida
{

	class TaskBatch;
	class Task;

	class TaskBatchRunResult
	{
	public:
		using TaskThroughputMap = std::unordered_map<std::string, TaskThroughput>;

		const std::string& getId() const
		{
			return _id;
		}

		const TaskBatch& getTaskBatch() const
		{
			return _taskBatch;
		}

		const TaskThroughputMap& getTasksThroughputs() const
		{
			return _tasksThroughputs;
		}

		void addResult(const Task& task, TaskThroughput&& throughput);

		explicit TaskBatchRunResult(const TaskBatch& taskBatch);
		virtual ~TaskBatchRunResult() = default;
	private:
		std::string _id;
	private:
		const TaskBatch& _taskBatch;
		TaskThroughputMap _tasksThroughputs;
	};
}


#endif //INCLUDE_ELPIDA_TASKBATCHRUNRESULT_HPP
