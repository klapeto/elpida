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
 * MemoryTasksProperties.hpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MEMORYTASKSPROPERTIES_HPP_
#define TASKBATCHES_MEMORY_MEMORYTASKSPROPERTIES_HPP_

#include "TaskBatches/QtTaskBatchWrapper.hpp"
#include "MemoryTaskBatch.hpp"

namespace QtCharts
{
	class QChartView;
	class QChart;
	class QLogValueAxis;
	class QValueAxis;
}  // namespace QtCharts

namespace Elpida
{

	class MemoryTasksProperties final: public QtTaskBatchWrapper
	{
		public:

			const TaskBatch& getTaskBatch() const
			{
				return _taskBatch;
			}

			void reconfigureTaskBatch()
			{

			}
			void validateConfiguration()
			{

			}

			QtCharts::QChart*  getResultsChartContainer() override
			{
				return _chart;
			}

			void updateResultsChartData(const Map<String, Array<TaskThroughput>>& results) override;

			MemoryTasksProperties();
			~MemoryTasksProperties();
		private:
			MemoryTaskBatch _taskBatch;
			QtCharts::QChart* _chart;
			QtCharts::QLogValueAxis* _xAxis;
			QtCharts::QValueAxis* _yAxis;

	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MEMORYTASKSPROPERTIES_HPP_ */
