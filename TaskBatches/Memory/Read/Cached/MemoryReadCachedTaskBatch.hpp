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

#ifndef TASKBATCHES_MEMORY_READ_CACHED_MEMORYREADCACHEDTASKBATCH_HPP_
#define TASKBATCHES_MEMORY_READ_CACHED_MEMORYREADCACHEDTASKBATCH_HPP_

#include <Elpida/TaskBatch.hpp>
#include <Elpida/Types/Primitives.hpp>

namespace Elpida
{

	class MemoryReadCachedTaskBatch : public TaskBatch
	{
		public:
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
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_CACHED_MEMORYREADCACHEDTASKBATCH_HPP_ */
