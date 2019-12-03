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
 * MultithreadMemoryChunksReadTaskBatch.hpp
 *
 *  Created on: 16 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYCHUNKSREADTASKBATCH_HPP_
#define TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYCHUNKSREADTASKBATCH_HPP_

#include "Elpida/TaskBatch.hpp"

namespace Elpida
{

	class MultithreadMemoryChunksReadTaskBatch: public TaskBatch
	{
		public:

			bool isAutoConfigureSizes() const
			{
				return _autoConfigureSizes;
			}

			void setAutoConfigureSizes(bool autoConfigureSizes)
			{
				_autoConfigureSizes = autoConfigureSizes;
			}

			std::size_t getSizePerThread() const
			{
				return _sizePerThread;
			}

			void setSizePerThread(std::size_t sizePerThread)
			{
				_sizePerThread = sizePerThread;
			}

			void createTasks() const override;

			MultithreadMemoryChunksReadTaskBatch()
					: TaskBatch("Memory Read Bandwidth"), _sizePerThread(256), _autoConfigureSizes(true)
			{

			}

			virtual ~MultithreadMemoryChunksReadTaskBatch()
			{
			}
		private:
			std::size_t _sizePerThread;
			bool _autoConfigureSizes;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MULTITHREADMEMORYCHUNKSREADTASKBATCH_HPP_ */
