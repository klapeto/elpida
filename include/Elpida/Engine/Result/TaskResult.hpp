//
// Created by klapeto on 19/4/20.
//

#ifndef INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP
#define INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP

#include <cstddef>
#include <utility>
#include "TaskMetrics.hpp"

namespace Elpida
{
	class TaskSpecification;

	class TaskResult final
	{
	public:
		[[nodiscard]] const TaskSpecification& getSpecification() const
		{
			return _specification;
		}
		[[nodiscard]] const TaskMetrics& getMetrics() const
		{
			return _metrics;
		}

		TaskResult(const TaskSpecification& specification, TaskMetrics&& metrics)
			: _specification(specification), _metrics(metrics)
		{

		}

		~TaskResult() = default;
	private:
		const TaskSpecification& _specification;
		TaskMetrics _metrics;
	};
}

#endif //INCLUDE_ELPIDA_ENGINE_RESULT_TASKRESULT_HPP
