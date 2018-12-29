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

			virtual void reconfigureTaskBatch() = 0;
			virtual void validateConfiguration() = 0;

			bool hasProperties() const
			{
				return _hasProperties;
			}

			virtual ~QtTaskBatchWrapper()
			{

			}
		protected:
			QtTaskBatchWrapper(bool hasProperties, QWidget* parrent = nullptr)
					: TaskBatchWrapper(), QWidget(parrent), _hasProperties(hasProperties)
			{

			}
		private:
			bool _hasProperties;
	};

} /* namespace Elpida */

#endif
#endif /* TASKBATCHES_QTTASKBATCHWRAPPER_HPP_ */
