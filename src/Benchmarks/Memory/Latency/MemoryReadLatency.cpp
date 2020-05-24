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
#include <Elpida/Engine/Data/PassiveTaskData.hpp>

namespace Elpida
{

	MemoryReadLatency::MemoryReadLatency(const TaskSpecification& specification,
		TaskAffinity affinity)
		: Task(specification, std::move(affinity)), _taskData(nullptr), _iterations(1)
	{

	}

	void MemoryReadLatency::prepareImpl()
	{
		_taskData = getInput().getTaskData();
		if (_taskData->getSize() % (32 * sizeof(RegisterSize)) != 0)
			throw ElpidaException("Memory size must be divisible by the size of each read * 32!");

		_iterations = (unsigned long)std::max(_iterationConstant / (double)_taskData->getSize(), 1.0);

		auto size = _taskData->getSize() / sizeof(RegisterSize);

		std::random_device rd;
		std::mt19937_64 generator(rd());
		std::uniform_int_distribution<size_t> distribution(0, size - 1);

		auto ptr = (RegisterSize*)_taskData->getData();
		for (std::size_t i = 0; i < size; ++i)
		{
			ptr[i] = distribution(generator);
		}
	}

	TaskOutput MemoryReadLatency::finalizeAndGetOutputData()
	{
		return TaskOutput(*new PassiveTaskData(*_taskData));
	}

	void MemoryReadLatency::execute()
	{
		volatile auto* ptr = (volatile RegisterSize*)_taskData->getData();
		const auto iterations = _iterations;
		const auto size = _taskData->getSize() / sizeof(RegisterSize);
		auto x = RegisterSize();
		for (auto i = 0ul; i < iterations; ++i)
		{
			for (auto j = 0ul; j < size; j += 32)
			{
				x = ptr[ptr[j]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 1]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 2]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 3]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 4]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 5]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 6]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 7]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 8]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 9]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 10]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 11]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 12]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 13]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 14]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 15]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 16]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 17]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 18]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 19]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 20]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 21]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 22]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 23]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 24]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 25]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 26]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 27]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 28]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 29]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 30]];
				asm volatile ("mfence":: : "memory");
				x = ptr[ptr[j + 31]];
				asm volatile ("mfence":: : "memory");
			}
		}
		ptr[x] = x;
	}

	double MemoryReadLatency::calculateTaskResultValue(const Duration& taskElapsedTime) const
	{
		return taskElapsedTime.count() / ((double)_iterations * ((double)_taskData->getSize() / sizeof(RegisterSize)));
	}

} /* namespace Elpida */
