//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKINPUT_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKINPUT_HPP

#include <utility>
#include <vector>

namespace Elpida
{
	class RawData;

	class TaskInput final
	{
	public:

		[[nodiscard]] RawData* getTaskData() const
		{
			return _taskData;
		}

		TaskInput();
		explicit TaskInput(RawData& taskData);
		TaskInput(const TaskInput&) = delete;
		TaskInput& operator=(const TaskInput&) = delete;
		TaskInput(TaskInput&& other) noexcept;
		TaskInput& operator=(TaskInput&& other) noexcept;
		~TaskInput();
	private:
		RawData* _taskData;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKINPUT_HPP
