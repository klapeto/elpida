//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP

#include <cstddef>
#include <utility>
#include <string>
#include "TaskMetrics.hpp"

namespace Elpida
{
	class TaskSpecification;
	class ResultSpecification;

	class TaskResult final
	{
	public:
		[[nodiscard]] const TaskSpecification& getTaskSpecification() const
		{
			return *_taskSpecification;
		}

		[[nodiscard]] const TaskMetrics& getMetrics() const
		{
			return _metrics;
		}

		TaskResult(const TaskSpecification& specification, TaskMetrics&& metrics);
		TaskResult(TaskResult&&) = default;
		TaskResult& operator=(TaskResult&&) = default;
		TaskResult(const TaskResult&) = default;
		TaskResult& operator=(const TaskResult&) = default;
		~TaskResult() = default;
	private:
		TaskMetrics _metrics;
		const TaskSpecification* _taskSpecification;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP
