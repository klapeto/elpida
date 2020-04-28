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
 * MemoryRead.cpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include <Elpida/ElpidaException.hpp>
#include "Benchmarks/Memory/Read/MemoryRead.hpp"

namespace Elpida
{

	MemoryRead::MemoryRead(const TaskSpecification& specification,
		const TaskAffinity& affinity,
		bool toBeCountedOnResults)
		: Task(specification, affinity, toBeCountedOnResults), _taskData(nullptr), _iterations(1)
	{

	}
	void MemoryRead::prepareImpl()
	{
		_taskData = getInput().getTaskData().front();
		if (_taskData->getSize() % (32 * sizeof(RegisterSize)) != 0)
			throw ElpidaException("Memory size must be divisible by the size of each read * 32!");

		_iterations = (unsigned long)(_iterationConstant / (double)_taskData->getSize());
	}

	TaskOutput MemoryRead::finalizeAndGetOutputData()
	{
		return TaskOutput(getInput().createPassiveWrappers());
	}

	size_t MemoryRead::getActualProcessedDataSize() const
	{
		return _taskData->getSize() * _iterations;
	}
} /* namespace Elpida */

