//
// Created by klapeto on 12/3/2023.
//


#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/OsUtilities.hpp"

#include <cerrno>
#include <cstring>
#include <numa.h>

namespace Elpida
{

	String OsUtilities::GetLastErrorString()
	{
		return { strerror(errno) };
	}

	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		if (numa_available() < 0) return 0;
		return numa_node_of_cpu(processorId);
	}

} // Elpida


#endif // defined(ELPIDA_UNIX)