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
 * MultithreadMemoryChunksReadCachedTaskBatch.hpp
 *
 *  Created on: 16 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MULTITHREADMEMORYCHUNKSREADCACHEDTASKBATCH_HPP_
#define TASKBATCHES_MEMORY_MULTITHREADMEMORYCHUNKSREADCACHEDTASKBATCH_HPP_

#include <Elpida/TaskBatch.hpp>
#include <Elpida/Types/Primitives.hpp>

namespace Elpida
{

	class MultithreadMemoryChunksReadCachedTaskBatch: public TaskBatch
	{
		public:
			void createTasks() const override;

			MultithreadMemoryChunksReadCachedTaskBatch()
					: TaskBatch("Memory Read (Multi Thread/Chunked/Cached)")
			{

			}

			virtual ~MultithreadMemoryChunksReadCachedTaskBatch()
			{
			}
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MULTITHREADMEMORYCHUNKSREADCACHEDTASKBATCH_HPP_ */