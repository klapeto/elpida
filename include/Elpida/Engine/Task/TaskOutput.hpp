//
// Created by klapeto on 26/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_TASK_TASKOUTPUT_HPP
#define INCLUDE_ELPIDA_ENGINE_TASK_TASKOUTPUT_HPP

#include "Elpida/Utilities/RawData.hpp"
#include "TaskDataDto.hpp"
#include <vector>

namespace Elpida
{
	class TaskOutput
	{
	public:
		[[nodiscard]] const RawData& getTaskData() const
		{
			return *_taskData;
		}

		TaskOutput();
		explicit TaskOutput(RawData& data);
		TaskOutput(const TaskOutput&) = delete;
		TaskOutput& operator=(const TaskOutput&) = delete;
		TaskOutput(TaskOutput&& other) noexcept ;
		TaskOutput& operator=(TaskOutput&& other) noexcept ;
		virtual ~TaskOutput();
	private:
		RawData* _taskData;
	};
}


#endif //INCLUDE_ELPIDA_ENGINE_TASK_TASKOUTPUT_HPP
