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

#include "Elpida/TaskBatchWrapper.hpp"
#include "Elpida/TaskThroughput.hpp"

#include <QWidget>
#include <vector>
#include <string>
#include <unordered_map>

namespace QtCharts {
	class QChart;
}  // namespace QtCharts

namespace Elpida
{
	class TaskBatch;

	class QtTaskBatchWrapper: public TaskBatchWrapper, public QWidget
	{
		public:

			virtual void reconfigureTaskBatch() = 0;
			virtual void validateConfiguration() = 0;

			virtual QtCharts::QChart* getResultsChartContainer()
			{
				return nullptr;
			}

			virtual void updateResultsChartData(const std::unordered_map<std::string, std::vector<TaskThroughput>>& results)
			{

			}

			bool hasProperties() const
			{
				return _hasProperties;
			}

			bool hasResultChart() const
			{
				return _hasResultCharts;
			}

			virtual ~QtTaskBatchWrapper()
			{

			}
		protected:
			QtTaskBatchWrapper(bool hasProperties, bool hasResultCharts, QWidget* parrent = nullptr)
					: TaskBatchWrapper(), QWidget(parrent), _hasProperties(hasProperties), _hasResultCharts(hasResultCharts)
			{

			}
		private:
			bool _hasProperties;
			bool _hasResultCharts;
	};

} /* namespace Elpida */
#endif /* TASKBATCHES_QTTASKBATCHWRAPPER_HPP_ */
