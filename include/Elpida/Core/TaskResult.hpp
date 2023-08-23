//
// Created by klapeto on 27/2/2023.
//

#ifndef ELPIDA_TASKRESULT_HPP_
#define ELPIDA_TASKRESULT_HPP_

#include "Elpida/Core/Duration.hpp"
#include "Elpida/Core/TaskInfo.hpp"
#include "Elpida/Core/Size.hpp"

namespace Elpida
{

	class TaskResult final
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

		TaskResult(const TaskResult&) = delete;
		TaskResult(TaskResult&&) = default;
		~TaskResult() = default;
	 private:
		Duration _duration;
		Size _inputSize;
	};

} // Elpida

#endif //ELPIDA_TASKRESULT_HPP_
