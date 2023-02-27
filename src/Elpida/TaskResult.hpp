//
// Created by klapeto on 27/2/2023.
//

#ifndef _TASKRESULT_HPP_
#define _TASKRESULT_HPP_

#include <cstdlib>

#include "Elpida/Duration.hpp"

namespace Elpida
{

	class TaskResult
	{
	 public:
		const Duration& GetDuration() const
		{
			return _duration;
		}

		size_t GetInputSize() const
		{
			return _inputSize;
		}

		TaskResult(const Duration& duration, std::size_t inputSize)
			: _duration(duration), _inputSize(inputSize)
		{
		}

		TaskResult(const TaskResult&) = default;
		TaskResult(TaskResult&&) = default;
		~TaskResult() = default;
	 private:
		Duration _duration;
		std::size_t _inputSize;
	};

} // Elpida

#endif //_TASKRESULT_HPP_
