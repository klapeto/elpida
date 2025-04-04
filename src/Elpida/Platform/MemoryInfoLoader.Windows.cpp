//
// Created by klapeto on 14/3/2023.
//

#include "Elpida/Core/Config.hpp"

#if defined(ELPIDA_WINDOWS)

#include "Elpida/Platform/MemoryInfoLoader.hpp"

#include <windows.h>

namespace Elpida
{

	MemoryInfo MemoryInfoLoader::Load()
	{
		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof(memStatus);
		GlobalMemoryStatusEx(&memStatus);
		auto totalSize = memStatus.ullTotalPhys;

		SYSTEM_INFO sysInfo;
		GetNativeSystemInfo(&sysInfo);
		auto pageSize = sysInfo.dwPageSize;

		return {
			static_cast<Size>(totalSize),
			static_cast<Size>(pageSize)
		};
	}
} // Elpida

#endif // defined(ELPIDA_WINDOWS)