//
// Created by klapeto on 28/2/2023.
//

#include "Elpida/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/ThreadTask.hpp"
#include "Elpida/ElpidaException.hpp"

#include <sched.h>
#include <cstring>

namespace Elpida
{
	void ThreadTask::PinCurrentThreadToProcessor(const TopologyNode& topologyNode)
	{
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(topologyNode.GetOsIndex().value(), &mask);

		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			throw ElpidaException("Failed to pin thread to: ", topologyNode.GetOsIndex().value(), ": ", strerror(errno));
		}
	}
}

#endif