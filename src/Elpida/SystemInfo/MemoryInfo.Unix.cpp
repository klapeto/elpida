/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2023  Ioannis Panagiotopoulos
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

//
// Created by klapeto on 25/2/2023.
//

#include "Elpida/Config.hpp"

#if defined(ELPIDA_UNIX)

#include "Elpida/SystemInfo/MemoryInfo.hpp"

#include <unistd.h>

namespace Elpida
{
	std::size_t MemoryInfo::getAvailableFreeMemory() const
	{
		return sysconf(_SC_AVPHYS_PAGES) * _pageSize;
	}

	void MemoryInfo::getValues()
	{
		_pageSize = sysconf(_SC_PAGESIZE);
		_memorySize = sysconf(_SC_PHYS_PAGES) * _pageSize;
	}
}

#endif