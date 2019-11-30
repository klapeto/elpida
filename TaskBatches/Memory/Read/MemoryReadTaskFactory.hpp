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
	template<typename T>
	class MemoryReadTaskFactory final: public TaskFactory
	{
		public:

			const std::vector<MemoryRead<T>*>& getCreatedTasks() const
			{
				return _createdTasks;
			}

			void resetCreatedTasks()
			{
				_createdTasks.clear();
			}

			std::size_t getSizePerTask() const
			{
				return _sizePerTask;
			}

			Task* create(const ProcessorNode& processorAffinity) const override
			{
				auto task = new MemoryRead<>(_sizePerTask, processorAffinity.getOsIndex());
				_createdTasks.push_back(task);
				return task;
			}

			MemoryReadTaskFactory(std::size_t sizePerTask)
					: _sizePerTask(sizePerTask)
			{

			}
			~MemoryReadTaskFactory()
			{

			}

		private:
			mutable std::vector<MemoryRead<T>*> _createdTasks;
			std::size_t _sizePerTask;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MEMORYREADTASKFACTORY_HPP_ */
