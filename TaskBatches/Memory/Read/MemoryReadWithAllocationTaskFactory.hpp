/*
 * MemoryReadWithAllocationTaskFactory.hpp
 *
 *  Created on: 30 Νοε 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_READ_MEMORYREADWITHALLOCATIONTASKFACTORY_HPP_
#define TASKBATCHES_MEMORY_READ_MEMORYREADWITHALLOCATIONTASKFACTORY_HPP_

#include "Elpida/TaskFactory.hpp"
#include <cstddef>
#include <vector>

namespace Elpida
{
	class MemoryReadWithAllocation;

	class MemoryReadWithAllocationTaskFactory final: public TaskFactory
	{
		public:

			const std::vector<MemoryReadWithAllocation*>& getCreatedTasks() const
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

			Task* create(const ProcessorNode& processorAffinity) const override;

			MemoryReadWithAllocationTaskFactory(std::size_t sizePerTask)
					: _sizePerTask(sizePerTask)
			{

			}
			~MemoryReadWithAllocationTaskFactory()
			{

			}

		private:
			mutable std::vector<MemoryReadWithAllocation*> _createdTasks;
			std::size_t _sizePerTask;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_READ_MEMORYREADWITHALLOCATIONTASKFACTORY_HPP_ */
