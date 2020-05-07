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
 * MemoryLatench.cpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#include "Benchmarks/Memory/Latency/MemoryReadLatency.hpp"
#include <Elpida/ElpidaException.hpp>
#include <utility>
#include <ctime>

namespace Elpida
{

	void MemoryReadLatency::prepareImpl()
	{
		_taskData = getInput().getTaskData().front();
		if (_taskData->getSize() % (32 * sizeof(RegisterSize)) != 0)
			throw ElpidaException("Memory size must be divisible by the size of each read * 32!");

		_iterations = (unsigned long)(_iterationConstant / (double)_taskData->getSize());

		std::srand(std::time(nullptr));
		auto size = _taskData->getSize() / sizeof(RegisterSize);

		auto ptr = (RegisterSize*)_taskData->getData();
		for (std::size_t i = 0; i < size; ++i)
		{
			ptr[i] = std::rand() / ((RAND_MAX + 1u) / (size - 1));
		}

	}
	MemoryReadLatency::MemoryReadLatency(const TaskSpecification& specification,
		TaskAffinity affinity,
		bool toBeCountedOnResults)
		: Task(specification, std::move(affinity), toBeCountedOnResults), _taskData(nullptr), _iterations(1)
	{

	}

	size_t MemoryReadLatency::getActualProcessedDataSize() const
	{
		return 0;
	}

	TaskOutput MemoryReadLatency::finalizeAndGetOutputData()
	{
		return TaskOutput();
	}

} /* namespace Elpida */
