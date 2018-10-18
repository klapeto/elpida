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
 * MemoryTaskBatch.cpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/Memory/MemoryTaskBatch.hpp"
#include "MemoryRead.hpp"
#include <Elpida/Types/Integer.hpp>

namespace Elpida
{
	template<Size N>
	struct SizedStruct
	{
			Int8 x[N];
	};

	void MemoryTaskBatch::createTasks() const
	{
		addTask(new MemoryRead<SizedStruct<1>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<1> ))));
		addTask(new MemoryRead<SizedStruct<2>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<2> ))));
		addTask(new MemoryRead<SizedStruct<4>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<4> ))));
		addTask(new MemoryRead<SizedStruct<8>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<8> ))));
		addTask(new MemoryRead<SizedStruct<16>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<16> ))));
		addTask(new MemoryRead<SizedStruct<32>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<32> ))));
		addTask(new MemoryRead<SizedStruct<64>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<64> ))));
		addTask(new MemoryRead<SizedStruct<128>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<128> ))));
		addTask(new MemoryRead<SizedStruct<256>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<256> ))));
		addTask(new MemoryRead<SizedStruct<512>>(((1024u * 1024 * 1024) / sizeof(SizedStruct<512> ))));
	}

} /* namespace Elpida */
