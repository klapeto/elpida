/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2022  Ioannis Panagiotopoulos
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

#include "Elpida/Utilities/NumaMemory.hpp"

#include "Elpida/Common/ElpidaException.hpp"
#include "Elpida/Utilities/OsUtilities.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include <numa.h>

namespace Elpida
{
	void* NumaMemory::allocateOnNumaNode(std::size_t size, int numaNode)
	{
		if (numa_available() < 0)
		{
			return malloc(size);
		}

		numa_set_strict(1);
		auto ptr = (void*)numa_alloc_onnode(size, numaNode);
		if (ptr == nullptr)
		{
			throw ElpidaException(FUNCTION_NAME,
					Vu::Cs("Failed to allocate to numa node ", numaNode, ". Memory of size: ",
							Vu::getValueScaleStringIEC(size), "B. Error: ", OsUtilities::GetLastErrorString()));
		}

		return ptr;
	}

	void* NumaMemory::deallocateOnNumaNode(void* ptr, std::size_t size)
	{
		if (numa_available() < 0)
		{
			free(ptr);
		}
		else
		{
			numa_free(ptr, size);
		}
	}

}

#endif