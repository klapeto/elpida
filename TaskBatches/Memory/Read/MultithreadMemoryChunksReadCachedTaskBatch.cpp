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
 * MultithreadMemoryChunksReadCached.cpp
 *
 *  Created on: 16 Μαΐ 2019
 *      Author: klapeto
 */

#include "TaskBatches/Memory/Read/MultithreadMemoryChunksReadCachedTaskBatch.hpp"

#include "Elpida/Topology/SystemTopology.hpp"
#include "TaskBatches/General/AllocateMemory.hpp"
#include "TaskBatches/Memory/Read/MultiThreadMemoryChunksRead.hpp"

namespace Elpida
{

	void MultithreadMemoryChunksReadCachedTaskBatch::createTasks() const
	{
		constexpr int size = 512 * (1024 * 1024);
		auto memory = new AllocateMemory(size, true, sizeof(void*) * 16);
		memory->setToBeMeasured(false);
		addTask(memory);
		addTask(new MultiThreadMemoryChunksRead(memory->getMemory(), SystemTopology::getTopology().getTotalLogicalCores()));
	}

} /* namespace Elpida */

