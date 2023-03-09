//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASKRESULT_HPP_
#define _TASKRESULT_HPP_

#include "Elpida/Duration.hpp"
#include "Elpida/TaskInfo.hpp"
#include "Elpida/Size.hpp"

namespace Elpida
{

	class TaskResult
	{
	 public:
		[[nodiscard]]
		const TaskInfo& GetTaskInfo() const
		{
			return _taskInfo;
		}

		[[nodiscard]]
		const Duration& GetDuration() const
		{
			return _duration;
		}

		[[nodiscard]]
		Size GetInputSize() const
		{
			return _inputSize;
		}

		TaskResult(TaskInfo&& taskInfo, const Duration& duration, Size inputSize)
			: _taskInfo(std::move(taskInfo)), _duration(duration), _inputSize(inputSize)
		{
		}

		TaskResult(const TaskResult&) = default;
		TaskResult(TaskResult&&) = default;
		~TaskResult() = default;
	 private:
		TaskInfo _taskInfo;
		Duration _duration;
		Size _inputSize;
	};

} // Elpida

#endif //_TASKRESULT_HPP_
