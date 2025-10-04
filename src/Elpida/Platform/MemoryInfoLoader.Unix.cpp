/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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