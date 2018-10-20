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
#include "MultiThreadMemoryRead.hpp"
#include "MemoryRead.hpp"
#include "TaskBatches/General/AllocateMemory.hpp"
#include <Elpida/Types/SizedStruct.hpp>
#include <Elpida/CpuInfo.hpp>
#include <Elpida/MemoryInfo.hpp>

namespace Elpida
{
	void MemoryTaskBatch::createTasks() const
	{
		const Size cores= CpuInfo::getCpuInfo().getLogicalProcessors();
		const auto& memoryInfo =MemoryInfo::getInfo();
		const Size maxMemory = memoryInfo.getMemorySize() >> 1;
		Size currentFreeMemory = memoryInfo.getAvailableFreeMemory();
		Size memoryToBeUsed = maxMemory;

		if (currentFreeMemory < maxMemory){
			memoryToBeUsed = currentFreeMemory >> 1;
		}
		auto memory = new AllocateMemory(memoryToBeUsed, true, 64);
		memory->setToBeMeasured(false);
		addTask(memory);
		addTask(new MemoryRead<SizedStruct<1>>(*memory));
		addTask(new MemoryRead<SizedStruct<2>>(*memory));
		addTask(new MemoryRead<SizedStruct<4>>(*memory));
		addTask(new MemoryRead<SizedStruct<8>>(*memory));
		addTask(new MemoryRead<SizedStruct<16>>(*memory));
		addTask(new MemoryRead<SizedStruct<32>>(*memory));
		addTask(new MemoryRead<SizedStruct<64>>(*memory));

		//addTask(new MultiThreadMemoryRead((sz) / sizeof(SizedStruct<64>)));
	}

} /* namespace Elpida */
