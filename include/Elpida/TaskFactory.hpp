/*
 * TaskFactory.hpp
 *
 *  Created on: 29 Νοε 2019
 *      Author: klapeto
 */

#ifndef ELPIDA_TASKFACTORY_HPP_
#define ELPIDA_TASKFACTORY_HPP_

namespace Elpida
{
	class Task;
	class ProcessorNode;

	class TaskFactory
	{
		public:

			virtual Task* create(const ProcessorNode& processorAffinity) const = 0;

			TaskFactory()
			{

			}
			virtual ~TaskFactory()
			{

			}
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKFACTORY_HPP_ */
