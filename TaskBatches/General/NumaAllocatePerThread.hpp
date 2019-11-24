/*
 * NumaAllocatePerThread.hpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_GENERAL_NUMAALLOCATEPERTHREAD_HPP_
#define TASKBATCHES_GENERAL_NUMAALLOCATEPERTHREAD_HPP_

#include <Elpida/Task.hpp>
#include <Elpida/TaskRunResult.hpp>
#include <Elpida/Types/Map.hpp>

namespace Elpida
{
	class Memory;

	class NumaAllocatePerThread: public Task
	{
		public:

			const Map<int, Memory*>& getAllocatedMemoryRegions() const
			{
				return _allocatedMemoryRegions;
			}

			void prepare() override;

			void run() override;
			void calculateResults(const TaskMetrics& metrics) override;

			NumaAllocatePerThread(Size memorySizePerThread);
			~NumaAllocatePerThread();

		private:
			Map<int, Memory*> _allocatedMemoryRegions;
			TaskRunResult _result;
			Size _memorySizePerThread;

	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_NUMAALLOCATEPERTHREAD_HPP_ */
