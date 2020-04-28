//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKINPUT_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKINPUT_HPP

#include <utility>
#include <vector>

namespace Elpida
{
	class TaskData;

	class TaskInput final
	{
	public:

		[[nodiscard]] const std::vector<TaskData*>& getTaskData() const
		{
			return _taskData;
		}


		[[nodiscard]] std::vector<TaskData*> createPassiveWrappers() const;

		TaskInput() = default;
		explicit TaskInput(std::vector<TaskData*> taskData)
			: _taskData(std::move(taskData))
		{
		}
		TaskInput(const TaskInput&) = delete;
		TaskInput& operator=(const TaskInput&) = delete;
		TaskInput(TaskInput&&) = default;
		TaskInput& operator=(TaskInput&&) = default;
		~TaskInput();
	private:
		std::vector<TaskData*> _taskData;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKINPUT_HPP
