//
// Created by klapeto on 19/4/20.
//


#include "Elpida/Engine/Task/Task.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Task/TaskThread.hpp"
#include "Elpida/Engine/Task/TaskSpecification.hpp"
#include "Elpida/ElpidaException.hpp"

namespace Elpida
{

	Task::Task(const TaskSpecification& specification, TaskAffinity affinity, bool toBeCountedOnResults)
		: _affinity(std::move(affinity)), _specification(specification),
		  _toBeCountedOnResults(toBeCountedOnResults)
	{
		if (_affinity.getProcessorNodes().empty())
		{
			throw ElpidaException(FUNCTION_NAME, "Task Affinity is empty!");
		}
	}

	void Task::applyAffinity()
	{
		auto nodes = _affinity.getProcessorNodes();
		if (!nodes.empty())
		{
			TaskThread::setCurrentThreadAffinity((int)nodes[0]->getOsIndex());
		}
	}

	void Task::prepare()
	{
		if (_specification.acceptsInput() && _inputData.getTaskData().empty())
		{
			throw ElpidaException(FUNCTION_NAME,
				Vu::Cs("'", _specification.getName(), "' task needs input and it was not provided!"));
		}
		prepareImpl();
	}

	void Task::finalize()
	{
		_outputData = finalizeAndGetOutputData();
	}

	TaskResult Task::calculateTaskResult(const Duration& taskElapsedTime) const
	{
		return TaskResult(_specification, TaskMetrics(taskElapsedTime, calculateTaskResultValue(taskElapsedTime)));
	}
}