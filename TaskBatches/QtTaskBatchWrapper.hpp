/*			virtual const TaskBatch& getTaskBatch() const = 0;
 * QtTaskBatchConfigurationGenerator.hpp
 *
 *  Created on: 6 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_QTTASKBATCHWRAPPER_HPP_
#define TASKBATCHES_QTTASKBATCHWRAPPER_HPP_

#include "TaskBatches/Config.hpp"
#if _elpida_qt_enabled

#include "Elpida/TaskBatchWrapper.hpp"
#include <QWidget>

namespace Elpida
{
	class TaskBatch;

	class QtTaskBatchWrapper: public TaskBatchWrapper, public QWidget
	{
		public:

			QtTaskBatchWrapper(QWidget* parrent = nullptr)
					: TaskBatchWrapper(), QWidget(parrent)
			{

			}
			virtual ~QtTaskBatchWrapper()
			{

			}
	};

} /* namespace Elpida */

#endif
#endif /* TASKBATCHES_QTTASKBATCHWRAPPER_HPP_ */
