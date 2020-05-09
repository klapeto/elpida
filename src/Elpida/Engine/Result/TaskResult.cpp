//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Result/TaskResult.hpp"

namespace Elpida
{


	TaskResult::TaskResult(const TaskSpecification& specification, TaskMetrics&& metrics)
		: _metrics(metrics), _taskSpecification(&specification)
	{

	}
}