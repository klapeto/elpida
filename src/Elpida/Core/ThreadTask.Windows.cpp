//
// Created by klapeto on 28/2/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Core/ThreadTask.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>

namespace Elpida
{
	void ThreadTask::PinCurrentThreadToProcessor(const TopologyNode& topologyNode)
	{
        auto fail = !SetThreadAffinityMask(GetCurrentThread(), 1 << (int)topologyNode.GetOsIndex().value());
		if (fail)
		{
			throw ElpidaException("Failed to pin thread to: ", topologyNode.GetOsIndex().value());
		}
	}
}

#endif