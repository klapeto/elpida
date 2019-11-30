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
 * MultiThreadMemoryChunksRead.cpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/Memory/Read/MultiThreadMemoryChunksRead.hpp"
#include "TaskBatches/Memory/Read/MemoryReadWithAllocation.hpp"

namespace Elpida
{
	void MultiThreadMemoryChunksRead::calculateResults(const TaskMetrics& metrics)
	{
		std::size_t totalItterations = 1;
		auto& tasks = _taskFactory.getCreatedTasks();
		if (tasks.size() > 0)
		{
			totalItterations = (*tasks.begin())->getIterations();
		}
		_totalBandwidth.setOriginalValue(_taskFactory.getSizePerTask() * tasks.size());
		_totalBandwidth.setMultiplier(totalItterations);
		addResult(_totalBandwidth);
		_taskFactory.resetCreatedTasks();
	}
} /* namespace Elpida */

