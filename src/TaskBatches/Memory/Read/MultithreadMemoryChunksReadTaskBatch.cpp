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

#include "TaskBatches/Memory/Read/MultithreadMemoryChunksReadTaskBatch.hpp"

#include <cstddef>
#include <unordered_map>

#include "Elpida/Topology/SystemTopology.hpp"
#include "Elpida/Topology/CpuInfo.hpp"
#include "Elpida/CommonTasks/NumaAllocatePerThread.hpp"
#include "TaskBatches/Memory/Read/MultiThreadMemoryChunksRead.hpp"

#include <algorithm>

namespace Elpida
{

	void MultithreadMemoryChunksReadTaskBatch::createTasks() const
	{
		addTask(new MultiThreadMemoryChunksRead(_sizePerThread));
	}

} /* namespace Elpida */

