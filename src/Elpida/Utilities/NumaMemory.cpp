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
 * NumaMemory.cpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#include "Elpida/Utilities/NumaMemory.hpp"

#include <cstring>
#include "Elpida/Config.hpp"
#include "Elpida/SystemInfo/ProcessorNode.hpp"

namespace Elpida
{
	NumaMemory::NumaMemory(std::size_t size, int numaNode)
		: Memory(size), _numaNode(numaNode)
	{
	}

	NumaMemory::~NumaMemory()
	{
		deallocate(); // TODO: this is not needed, but be safe
	}

	void NumaMemory::allocateImpl()
	{
		_pointer = (pData)allocateOnNumaNode(_size, _numaNode);
		std::memset(_pointer, 0, _size);
	}

	void NumaMemory::deallocateImpl()
	{
		deallocateOnNumaNode(_pointer, _size);
	}

} /* namespace Elpida */

