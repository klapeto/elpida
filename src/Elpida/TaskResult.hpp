//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASKRESULT_HPP_
#define _TASKRESULT_HPP_

#include "Elpida/Duration.hpp"
#include "Elpida/Size.hpp"

namespace Elpida
{

	class TaskResult
	{
	 public:
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

		TaskResult(const Duration& duration, Size inputSize)
			: _duration(duration), _inputSize(inputSize)
		{
		}

		TaskResult(const TaskResult&) = default;
		TaskResult(TaskResult&&) = default;
		~TaskResult() = default;
	 private:
		Duration _duration;
		Size _inputSize;
	};

} // Elpida

#endif //_TASKRESULT_HPP_
