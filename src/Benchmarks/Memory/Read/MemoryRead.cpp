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

	MemoryRead::MemoryRead(const TaskSpecification& specification, const TaskAffinity& affinity)
		: Task(specification, affinity), _taskData(nullptr), _iterations(1)
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

	size_t MemoryRead::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return _taskData->getSize() * _iterations;;
	}

	void MemoryRead::execute()
	{
		volatile auto* ptr = (volatile RegisterSize*)_taskData->getData();
		volatile RegisterSize* const start = ptr;
		volatile const RegisterSize* const end = (RegisterSize*)((RegisterSize)start + _taskData->getSize());
		const auto iterations = _iterations;
		auto x = RegisterSize();
		for (auto i = 0ul; i < iterations; ++i)
		{
			ptr = start;
			while (ptr < end)
			{
				x = *ptr;
				x = *(ptr + 1);
				x = *(ptr + 2);
				x = *(ptr + 3);
				x = *(ptr + 4);
				x = *(ptr + 5);
				x = *(ptr + 6);
				x = *(ptr + 7);
				x = *(ptr + 8);
				x = *(ptr + 9);
				x = *(ptr + 10);
				x = *(ptr + 11);
				x = *(ptr + 12);
				x = *(ptr + 13);
				x = *(ptr + 14);
				x = *(ptr + 15);
				x = *(ptr + 16);
				x = *(ptr + 17);
				x = *(ptr + 18);
				x = *(ptr + 19);
				x = *(ptr + 20);
				x = *(ptr + 21);
				x = *(ptr + 22);
				x = *(ptr + 23);
				x = *(ptr + 24);
				x = *(ptr + 25);
				x = *(ptr + 26);
				x = *(ptr + 27);
				x = *(ptr + 28);
				x = *(ptr + 29);
				x = *(ptr + 30);
				x = *(ptr + 31);
				ptr += 32;
			}
		}
		auto dummy = x;
	}

} /* namespace Elpida */

