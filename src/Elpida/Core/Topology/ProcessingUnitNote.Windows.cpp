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
	void ProcessingUnitNode::PinThreadToThisProcessor() const
	{
        auto fail = !SetThreadAffinityMask(GetCurrentThread(), 1 << (int)GetOsIndex().value());
		if (fail)
		{
			throw ElpidaException("Failed to pin thread to: ", GetOsIndex().value());
		}
	}
}

#endif