/*
 * MultiThreadMemoryRead.hpp
 *
 *  Created on: 20 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MULTITHREADMEMORYREAD_HPP_
#define TASKBATCHES_MEMORY_MULTITHREADMEMORYREAD_HPP_

#include <Elpida/MultiThreadTask.hpp>
#include <Elpida/Types/Array.hpp>
#include <Elpida/Types/Primitives.hpp>

namespace Elpida
{

	class MultiThreadMemoryRead: public MultiThreadTask
	{
		public:

			void calculateResults();

			MultiThreadMemoryRead(Size size);
			virtual ~MultiThreadMemoryRead();

		protected:
			void createTasks();
		private:
			TaskRunResult _result;
			Size _size;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MULTITHREADMEMORYREAD_HPP_ */
