/*
 * TaskBatchConfigurationGenerator.hpp
 *
 *  Created on: 6 Οκτ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_TASKBATCHWRAPPER_HPP_
#define ELPIDA_TASKBATCHWRAPPER_HPP_

namespace Elpida
{
	class TaskBatch;

	class TaskBatchWrapper
	{
		public:

			virtual const TaskBatch& getTaskBatch() const = 0;

			TaskBatchWrapper()
			{

			}
			virtual ~TaskBatchWrapper()
			{

			}
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKBATCHWRAPPER_HPP_ */
