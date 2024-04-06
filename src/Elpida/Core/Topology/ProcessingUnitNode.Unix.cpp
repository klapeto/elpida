//
// Created by klapeto on 31/3/2024.
//


#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Core/Topology/ProcessingUnitNode.hpp"
#include "Elpida/Core/ElpidaException.hpp"

#include <sched.h>
#include <cstring>

namespace Elpida
{
	void ProcessingUnitNode::PinThreadToProcessor(unsigned int processorId)
	{
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(processorId, &mask);

		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			throw ElpidaException("Failed to pin thread to: ", processorId, ": ", strerror(errno));
		}
	}


	void ProcessingUnitNode::PinProcessToProcessors(const std::vector<Ref<const ProcessingUnitNode>>& processors)
	{
		cpu_set_t mask;
		CPU_ZERO(&mask);

		for (auto& processor: processors)
		{
			CPU_SET(processor.get().GetOsIndex().value(), &mask);
		}

		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			throw ElpidaException("Failed to pin thread to: ", strerror(errno));
		}
	}

} // Elpida

#endif    // defined(ELPIDA_UNIX)