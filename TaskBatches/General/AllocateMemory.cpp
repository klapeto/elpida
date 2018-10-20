/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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
 * AllocateMemory.cpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/General/AllocateMemory.hpp"
#include <Elpida/Config.hpp>
#include <cstdlib>
#include <cstring>

namespace Elpida
{

	AllocateMemory::AllocateMemory(Size size, bool initialize, int alignment)
			:
			  Task("Allocate Memory"),
			  _result("Allocation Rate", "Bytes"),
			  _size(size),
			  _data(nullptr),
			  _alignment(alignment),
			  _initialize(initialize)
	{
	}

	AllocateMemory::~AllocateMemory()
	{
		deallocate();
	}

	void AllocateMemory::run()
	{
		if (_alignment > 0)
		{
#if _elpida_linux
			posix_memalign(&_data, _alignment, _size);
#elif _elpida_windows
			_data = aligned_alloc(_alignment, _size);
#endif
		}
		else
		{
			_data = malloc(_size);
		}
	}

	void AllocateMemory::calculateResults()
	{
		_result = _size;
		addResult(_result);
	}

	void AllocateMemory::finalize()
	{
		if (_initialize && _data != nullptr)
		{
			memset(_data, 0, _size);
		}
	}

	void AllocateMemory::deallocate()
	{
		if (_data != nullptr)
		{
			free(_data);
			_data = nullptr;
		}
	}

} /* namespace Elpida */

