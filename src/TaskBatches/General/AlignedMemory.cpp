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
 * AlignedMemory.cpp
 *
 *  Created on: 21 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/General/AlignedMemory.hpp"
#include "Elpida/Config.hpp"
#include <cstdlib>
#include <cstring>

namespace Elpida
{

	void AlignedMemory::allocateImpl()
	{
#if _elpida_linux
		posix_memalign(&_pointer, _alignment, _size);
#elif _elpida_windows
		_pointer = _aligned_malloc(_size, _alignment);
#endif
		memset(_pointer, 0, _size);
	}

	void AlignedMemory::deallocateImpl()
	{

#if _elpida_linux
		free(_pointer);
#elif _elpida_windows
		_aligned_free(_pointer);
#endif
	}

} /* namespace Elpida */
