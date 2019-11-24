/*
 * NumaMemory.hpp
 *
 *  Created on: 9 Ιουν 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_GENERAL_NUMAMEMORY_HPP_
#define TASKBATCHES_GENERAL_NUMAMEMORY_HPP_

#include "TaskBatches/General/Memory.hpp"

namespace Elpida
{

	class NumaMemory final: public Memory
	{
		public:
			NumaMemory(Size size, int node)
					: Memory(size), _node(node)
			{
			}

			~NumaMemory()
			{
				deallocate();
			}
		private:
			int _node;
		protected:
			void allocateImpl() override;
			void deallocateImpl() override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_GENERAL_NUMAMEMORY_HPP_ */
