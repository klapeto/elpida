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
 * MemoryReadWithAllocationTaskFactory.hpp
 *
 *  Created on: 30 Νοε 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_MEMORYREADTASKFACTORY_HPP_
#define TASKBATCHES_MEMORY_READ_MEMORYREADTASKFACTORY_HPP_

#include "Elpida/TaskFactory.hpp"
#include "TaskBatches/Memory/Read/MemoryRead.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"
#include <vector>

namespace Elpida
{
	class MemoryReadTaskFactory final : public TaskFactory
	{
	public:

		std::size_t getSizePerTask() const
		{
			return _sizePerTask;
		}

		Task* create(const ProcessorNode& processorAffinity) const override
		{
			std::size_t size = 0;
			if (_autoConfigure)
			{
				// fuck
			}
			else
			{
				size = _sizePerTask;
			}

			auto task = new MemoryRead(size, processorAffinity.getOsIndex());
			_totalSize += size * task->getIterations();
			return task;
		}

		std::size_t getTotalSize() const
		{
			return _totalSize;
		}

		explicit MemoryReadTaskFactory(std::size_t sizePerTask)
			: _sizePerTask(sizePerTask), _totalSize(0), _autoConfigure(false)
		{

		}

		MemoryReadTaskFactory()
			: _sizePerTask(0), _totalSize(0), _autoConfigure(true)
		{

		}

		~MemoryReadTaskFactory() override = default;
	private:
		std::size_t _sizePerTask;
		mutable std::size_t _totalSize;
		bool _autoConfigure;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MEMORYREADTASKFACTORY_HPP_ */
