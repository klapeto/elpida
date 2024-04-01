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
	void ProcessingUnitNode::PinThreadToThisProcessor() const
	{
		cpu_set_t mask;
		CPU_ZERO(&mask);
		CPU_SET(GetOsIndex().value(), &mask);

		if (sched_setaffinity(0, sizeof(cpu_set_t), &mask))
		{
			throw ElpidaException("Failed to pin thread to: ", GetOsIndex().value(), ": ", strerror(errno));
		}
	}

} // Elpida

#endif	// defined(ELPIDA_UNIX)