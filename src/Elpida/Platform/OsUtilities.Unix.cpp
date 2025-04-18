//
// Created by klapeto on 12/3/2023.
//


#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/OsUtilities.hpp"

#include "Elpida/Core/ValueUtilities.hpp"

#include <cerrno>
#include <cstring>

#include <numa.h>

namespace Elpida
{

	using Vu = Elpida::ValueUtilities;

	String OsUtilities::GetLastErrorString()
	{
		return { strerror(errno) };
	}

	bool OsUtilities::ConvertArgumentsToUTF8(int& originalArgC, char**& originalArgV)
	{
		return true;
	}

	unsigned int OsUtilities::GetNumaNodeIdForProcessor(unsigned int processorId)
	{
		if (numa_available() < 0) return 0;
		return numa_node_of_cpu(processorId);
	}

	std::filesystem::path OsUtilities::GetExecutableDirectory()
	{
		return GetExecutablePath().parent_path();
	}

	std::filesystem::path OsUtilities::GetExecutablePath()
	{
		return std::filesystem::canonical("/proc/self/exe");
	}
} // Elpida


#endif // defined(ELPIDA_UNIX)