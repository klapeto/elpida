//
// Created by klapeto on 14/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/Platform/MemoryInfoLoader.hpp"

#include <unistd.h>

namespace Elpida
{

	MemoryInfo MemoryInfoLoader::Load()
	{
		auto pageSize = sysconf(_SC_PAGESIZE);
		auto totalSize = sysconf(_SC_PHYS_PAGES) * pageSize;

		return {
			static_cast<Size>(totalSize),
			static_cast<Size>(pageSize)
		};
	}
} // Elpida

#endif // defined(ELPIDA_UNIX)