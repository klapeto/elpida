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
 * Memory.cpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/Memory.hpp"

namespace Elpida
{
	Memory::Memory(std::size_t size)
		: _size(size), _pointer(nullptr)
	{

	}

	Memory::Memory(Memory&& other) noexcept
	{
		std::unique_lock<std::mutex> lock(other._mutex);
		this->_size = other._size;
		other._size = 0;
		this->_pointer = other._pointer;
		other._pointer = nullptr;
	}

	void Memory::allocate()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_pointer != nullptr)
		{
			deallocateImpl();
			_pointer = nullptr;
		}
		allocateImpl();
	}

	void Memory::deallocate()
	{
		std::unique_lock<std::mutex> lock(_mutex);
		if (_pointer != nullptr)
		{
			deallocateImpl();
			_pointer = nullptr;
		}
	}

	Memory::~Memory()
	{
		deallocate();
	}

} /* namespace Elpida */
