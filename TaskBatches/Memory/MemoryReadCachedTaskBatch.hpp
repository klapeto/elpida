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
 * MemoryReadCachedTaskBatch.hpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MEMORYREADCACHEDTASKBATCH_HPP_
#define TASKBATCHES_MEMORY_MEMORYREADCACHEDTASKBATCH_HPP_

#include <Elpida/TaskBatch.hpp>
#include <Elpida/Types/Primitives.hpp>

namespace Elpida
{

	class MemoryReadCachedTaskBatch : public TaskBatch
	{
		public:

			enum WorkingSetSize
			{
				B_128 = 128,
				B_256 = 256,
				B_512 = 512,
				KB_1 = 1024,
				KB_2 = KB_1 * 2,
				KB_4 = KB_1 * 4,
				KB_8 = KB_1 * 8,
				KB_16 = KB_1 * 16,
				KB_32 = KB_1 * 32,
				KB_64 = KB_1 * 64,
				KB_128 = KB_1 * 128,
				KB_256 = KB_1 * 256,
				KB_512 = KB_1 * 512,
				MB_1 = KB_1 * 1024,
				MB_2 = MB_1 * 2,
				MB_4 = MB_1 * 4,
				MB_8 = MB_1 * 8,
				MB_16 = MB_1 * 16,
				MB_32 = MB_1 * 32,
				MB_64 = MB_1 * 64,
				MB_128 = MB_1 * 128,
				MB_256 = MB_1 * 256,
				MB_512 = MB_1 * 512,
			};

			void onBeforeExecution() const override;
			void createTasks() const override;

			MemoryReadCachedTaskBatch()
					: TaskBatch("Memory Read (Single Thread/Cached)")
			{

			}

			MemoryReadCachedTaskBatch(const String& name)
					: TaskBatch(name)
			{

			}

			virtual ~MemoryReadCachedTaskBatch()
			{

			}

		protected:
			virtual void addMemoryReadTask(Size size) const;

			static constexpr int workingSetSize[] = {
			        B_128,
			        B_256,
			        B_512,
			        KB_1,
			        KB_2,
			        KB_4,
			        KB_8,
			        KB_16,
			        KB_32,
			        KB_64,
			        KB_128,
			        KB_256,
			        KB_512,
			        MB_1,
			        MB_2,
			        MB_4,
			        MB_8,
			        MB_16,
			        MB_32,
			        MB_64,
			        MB_128,
			        MB_256,
			        MB_512
			};

	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MEMORYREADCACHEDTASKBATCH_HPP_ */
