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
 * MultiThreadMemoryRead.cpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/Memory/MultiThreadMemoryRead.hpp"
#include "MemoryRead.hpp"
#include <Elpida/Types/SizedStruct.hpp>
#include <Elpida/CpuInfo.hpp>

namespace Elpida
{
	MultiThreadMemoryRead::MultiThreadMemoryRead(const Memory& memory)
			:
			  MultiThreadTask("Read " + ValueUtilities::getValueScale(memory.getSize()) + "B@64 Bytes/Read", true),
			  _result("Memory Read Rate", "Bytes"),
			  _memory(memory)
	{
	}

	MultiThreadMemoryRead::~MultiThreadMemoryRead()
	{
	}

	void MultiThreadMemoryRead::calculateResults()
	{
		addResult(_result);
	}

	void MultiThreadMemoryRead::createTasks()
	{
		auto cores = CpuInfo::getCpuInfo().getLogicalProcessors();
		for (int i = 0; i < cores; ++i)
		{
			addTask(new MemoryRead<SizedStruct<64>>(_memory));
		}
		_result.setMeasuredValue(cores * _memory.getSize());
	}

} /* namespace Elpida */

