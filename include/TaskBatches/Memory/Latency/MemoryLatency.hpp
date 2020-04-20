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
 * MemoryLatency.hpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_
#define TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_

#include <cstdint>
#include <string>
#include <random>

#include "Elpida/Task.hpp"
#include "Elpida/TaskMetrics.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"
#include "Elpida/CommonTasks/Memory.hpp"

namespace Elpida
{

	template<typename T = int64_t>
	class MemoryLatency : public Task
	{
	public:

		void prepare() override
		{
			std::srand(std::time(nullptr));
			auto size = _memory.getSize();

			auto ptr = (T*)_memory.getPointer();
			for (std::size_t i = 0; i < size; ++i)
			{
				ptr[i] = std::rand() / ((RAND_MAX + 1u) / (size - 1));
			}
		}

		void run() override
		{
			volatile auto ptr = (T*)_memory.getPointer();
			auto start = ptr;
			auto end = (T*)((T)start + _memory.getSize());
			auto iterations = _iterations;
			auto x = T();

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

		[[nodiscard]] unsigned long getIterations() const
		{
			return _iterations;
		}

		void calculateResults(const TaskMetrics& metrics) override
		{
			auto size = _iterations * _memory.getSize();
			auto time = metrics.getDurationSubdivision<TaskMetrics::NanoSecond>();
			_runResult.setOriginalValue((double)time / (double)size);
			_runResult.setCustom(true);
			_runResult.setMultiplier(1000);
			_runResult.setTestedDataValue(_memory.getSize());
			_runResult.setValueName("ns");
			addResult(_runResult);
		}

		explicit MemoryLatency(const Memory& memory)
			: Task("Memory Read latency: " + ValueUtilities::getValueScaleString(memory.getSize()) + "B"),
			  _memory(memory)
		{
			_iterations = _iterationConstant / (double)_memory.getSize();
		}

		~MemoryLatency() override = default;

	private:
		TaskRunResult _runResult;
	protected:
		const Memory& _memory;
		unsigned long _iterations;
	private:
		static constexpr double _iterationConstant =
			100000000000; // rough estimate, to be passed on construction later once we find the latency
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_LATENCY_MEMORYLATENCY_HPP_ */
