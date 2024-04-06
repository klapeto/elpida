//
// Created by klapeto on 31/3/2024.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <windows.h>

namespace Elpida
{
	void ProcessingUnitNode::PinThreadToProcessor(unsigned int processorId)
	{
		auto fail = !SetThreadAffinityMask(GetCurrentThread(), 1 << (int)processorId);
		if (fail)
		{
			throw ElpidaException("Failed to pin thread to: ", processorId);
		}
	}


	void ProcessingUnitNode::PinProcessToProcessors(const std::vector<Ref<const ProcessingUnitNode>>& processors)
	{
		DWORD_PTR affinity = 0;

		for (auto& processor: processors)
		{
			affinity |= 1 << (int)processor.get().GetOsIndex().value();
		}

		if (!SetProcessAffinityMask(GetCurrentProcess(), affinity))
		{
			throw ElpidaException("Failed to set process affinity");
		}
	}
}

#endif