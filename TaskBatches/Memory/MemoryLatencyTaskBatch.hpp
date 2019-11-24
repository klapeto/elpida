/*
 * MemoryLatencyTaskBatch.hpp
 *
 *  Created on: 19 Μαΐ 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MEMORYLATENCYTASKBATCH_HPP_
#define TASKBATCHES_MEMORY_MEMORYLATENCYTASKBATCH_HPP_

#include <Elpida/TaskBatch.hpp>
#include <Elpida/Types/Primitives.hpp>

namespace Elpida
{

	class MemoryLatencyTaskBatch final: public TaskBatch
	{
		public:

			void createTasks() const override;

			MemoryLatencyTaskBatch()
					: TaskBatch("Memory Latency")
			{

			}
			~MemoryLatencyTaskBatch()
			{
			}
		private:
			void addMemoryLatencyTask(Size size) const;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MEMORYLATENCYTASKBATCH_HPP_ */
