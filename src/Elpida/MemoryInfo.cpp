/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

/*
 * MemoryInfo.cpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/MemoryInfo.hpp"

#include "Elpida/Config.hpp"

#ifdef ELPIDA_WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#endif

namespace Elpida
{
	MemoryInfo::MemoryInfo()
		: _memorySize(0), _pageSize(0)
	{
		getValues();
	}

	std::size_t MemoryInfo::getAvailableFreeMemory() const
	{
#ifdef ELPIDA_LINUX
		return sysconf(_SC_AVPHYS_PAGES) * _pageSize;
#else
		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof(memStatus);
		GlobalMemoryStatusEx(&memStatus);
		return memStatus.ullAvailPhys;
#endif
	}

	void MemoryInfo::getValues()
	{
#ifdef ELPIDA_LINUX
		_pageSize = sysconf(_SC_PAGESIZE);
		_memorySize = sysconf(_SC_PHYS_PAGES) * _pageSize;
#else
		MEMORYSTATUSEX memStatus;
		memStatus.dwLength = sizeof(memStatus);
		GlobalMemoryStatusEx(&memStatus);
		_memorySize = memStatus.ullTotalPhys;

		SYSTEM_INFO sysInfo;
		GetNativeSystemInfo(&sysInfo);
		_pageSize = sysInfo.dwPageSize;
#endif
	}


} /* namespace Elpida */

