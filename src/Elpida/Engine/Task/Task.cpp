//
// Created by klapeto on 19/4/20.
//

#include "Elpida/Engine/Task/Task.hpp"

#include "Elpida/Topology/ProcessorNode.hpp"
#include "Elpida/Engine/Task/TaskThread.hpp"

namespace Elpida
{

	void Task::applyAffinity()
	{
		auto nodes = _affinity.getProcessorNodes();
		if (!nodes.empty())
		{
			auto id = nodes[0]->getOsIndex();
			if (id >= 0)
			{
				TaskThread::setCurrentThreadAffinity((int)id);
			}
		}
	}
}