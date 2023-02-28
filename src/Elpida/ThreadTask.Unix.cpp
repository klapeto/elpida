//
// Created by klapeto on 28/2/2023.
//

#include "Elpida/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/ThreadTask.hpp"

#include <sched.h>

namespace Elpida
{
	void ThreadTask::PinCurrentThreadToProcessor(int processorId)
	{
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(processorId, &mask);

		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			// throw;
		}
	}
}

#endif