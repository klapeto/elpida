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
 * MemoryRead.hpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MEMORYREAD_HPP_
#define TASKBATCHES_MEMORY_MEMORYREAD_HPP_

#include <Elpida/Task.hpp>
#include <Elpida/Types/Integer.hpp>
#include <Elpida/Types/Primitives.hpp>
#include <Elpida/TaskRunResult.hpp>
#include <Elpida/Utilities/ValueUtilities.hpp>
#include <TaskBatches/General/Memory.hpp>
#include <smmintrin.h>

namespace Elpida
{
	class MemoryRead final: public Task
	{
		public:
			void run() override
			{
				register auto ptr = (char*) _memory.getPointer();
				register auto start = ptr;
				register auto end = start + _memory.getSize();
				register auto itterations = _itterations;
				for (register auto i = 0ul; i < itterations; ++i)
				{
					ptr = start;
					while (ptr < end)
					{
#if __x86_64__
						asm("mov rax, [%0];"
								"mov rax, [%0 + 8];"
								"mov rax, [%0 + 16];"
								"mov rax, [%0 + 24];"
								"mov rax, [%0 + 32];"
								"mov rax, [%0 + 40];"
								"mov rax, [%0 + 48];"
								"mov rax, [%0 + 56];"
								"mov rax, [%0 + 64];"
								"mov rax, [%0 + 72];"
								"mov rax, [%0 + 80];"
								"mov rax, [%0 + 88];"
								"mov rax, [%0 + 96];"
								"mov rax, [%0 + 104];"
								"mov rax, [%0 + 112];"
								"mov rax, [%0 + 120];"
								"mov rax, [%0 + 128];"
								"mov rax, [%0 + 136];"
								"mov rax, [%0 + 144];"
								"mov rax, [%0 + 152];"
								"mov rax, [%0 + 160];"
								"mov rax, [%0 + 168];"
								"mov rax, [%0 + 176];"
								"mov rax, [%0 + 184];"
								"mov rax, [%0 + 192];"
								"mov rax, [%0 + 200];"
								"mov rax, [%0 + 208];"
								"mov rax, [%0 + 216];"
								"mov rax, [%0 + 224];"
								"mov rax, [%0 + 232];"
								"mov rax, [%0 + 240];"
								"mov rax, [%0 + 248];"::"r"(ptr));
#endif
						ptr += 256;
					}
				}
			}

			void calculateResults() override
			{
				addResult(_runResult);
			}

			MemoryRead(const Memory& memory, std::chrono::milliseconds duration)
					:
					  Task("Read " + ValueUtilities::getValueScale(memory.getSize()) + "B @8 Bytes/Read"),
					  _runResult("Memory Read Bandwidth", "Bytes"),
					  _memory(memory)
			{
				_itterations = (duration.count() / _secondsPerMov) / memory.getSize();
				_runResult.setMeasuredValue(_memory.getSize() * _itterations);
			}

			~MemoryRead()
			{
				finalize();
			}

		private:
			TaskRunResult _runResult;
			const Memory& _memory;
			unsigned long _itterations;
			static constexpr double _secondsPerMov = 10.0 / 1000000000.0;// rough estimate, to be passed on construction later once we find the latency
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MEMORYREAD_HPP_ */
