//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKOUTPUT_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKOUTPUT_HPP

#include "Elpida/Engine/Task/Data/TaskData.hpp"
#include "TaskInput.hpp"
#include <vector>

namespace Elpida
{
	class TaskOutput
	{
	public:
		[[nodiscard]] const std::vector<TaskData*>& getTaskData() const
		{
			return _taskData;
		}

		[[nodiscard]] TaskInput createTaskInput()
		{
			std::vector<TaskData*> data;
			for (auto& d : _taskData)
			{
				data.push_back(d);
			}
			return TaskInput(data);
		}

		TaskOutput() = default;

		explicit TaskOutput(TaskData* data)
		{
			_taskData.push_back(data);
		}

		explicit TaskOutput(std::vector<TaskData*>&& taskData)
			: _taskData(std::move(taskData))
		{
		}

		TaskOutput(const TaskOutput&) = delete;
		TaskOutput& operator=(const TaskOutput&) = delete;

		TaskOutput(TaskOutput&&) = default;
		TaskOutput& operator=(TaskOutput&&) = default;

		virtual ~TaskOutput();
	private:
		std::vector<TaskData*> _taskData;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKOUTPUT_HPP
