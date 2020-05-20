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
#include <Elpida/Utilities/Duration.hpp>
#include <utility>
#include <ctime>

namespace Elpida
{

	MemoryReadLatency::MemoryReadLatency(const TaskSpecification& specification,
		TaskAffinity affinity)
		: Task(specification, std::move(affinity)), _taskData(nullptr), _iterations(1)
	{

	}

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

	TaskOutput MemoryReadLatency::finalizeAndGetOutputData()
	{
		return TaskOutput();
	}

	void MemoryReadLatency::execute()
	{
		volatile auto* ptr = (volatile RegisterSize*)_taskData->getData();
		volatile RegisterSize* start = ptr;
		volatile auto* end = (volatile RegisterSize*)((unsigned char*)ptr + _taskData->getSize());
		const auto iterations = _iterations;
		auto x = RegisterSize();
		for (auto i = 0ul; i < iterations; ++i)
		{
			ptr = start;
			while (ptr < end)
			{
				x = ptr[*ptr];
				x = ptr[*(ptr + 1)];
				x = ptr[*(ptr + 2)];
				x = ptr[*(ptr + 3)];
				x = ptr[*(ptr + 4)];
				x = ptr[*(ptr + 5)];
				x = ptr[*(ptr + 6)];
				x = ptr[*(ptr + 7)];
				x = ptr[*(ptr + 8)];
				x = ptr[*(ptr + 9)];
				x = ptr[*(ptr + 10)];
				x = ptr[*(ptr + 11)];
				x = ptr[*(ptr + 12)];
				x = ptr[*(ptr + 13)];
				x = ptr[*(ptr + 14)];
				x = ptr[*(ptr + 15)];
				x = ptr[*(ptr + 16)];
				x = ptr[*(ptr + 17)];
				x = ptr[*(ptr + 18)];
				x = ptr[*(ptr + 19)];
				x = ptr[*(ptr + 20)];
				x = ptr[*(ptr + 21)];
				x = ptr[*(ptr + 22)];
				x = ptr[*(ptr + 23)];
				x = ptr[*(ptr + 24)];
				x = ptr[*(ptr + 25)];
				x = ptr[*(ptr + 26)];
				x = ptr[*(ptr + 27)];
				x = ptr[*(ptr + 28)];
				x = ptr[*(ptr + 29)];
				x = ptr[*(ptr + 30)];
				x = ptr[*(ptr + 31)];
				ptr += 32;
			}
		}
		auto dummy = x;
	}

	double MemoryReadLatency::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return taskElapsedTime.count() / ((double)_iterations * ((double)_taskData->getSize() / sizeof(RegisterSize)));
	}

} /* namespace Elpida */
