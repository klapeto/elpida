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
 * MemoryBandwidthChart.hpp
 *
 *  Created on: 24 Νοε 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_UI_MEMORYBANDWIDTHCHART_HPP_
#define TASKBATCHES_MEMORY_UI_MEMORYBANDWIDTHCHART_HPP_

#include "TaskBatches/Memory/Ui/MemoryTasksPropertiesWithChart.hpp"

namespace Elpida
{

	template<typename T>
	class MemoryBandwidthChart: public MemoryTasksPropertiesWithChart<T>
	{
		public:
			MemoryBandwidthChart(T* taskBatch)
					: MemoryTasksPropertiesWithChart<T>(taskBatch)
			{
			}
			virtual ~MemoryBandwidthChart()
			{
			}
		protected:
			MemoryBandwidthChart(T* taskBatch, bool hasProperties)
					: MemoryTasksPropertiesWithChart<T>(taskBatch, hasProperties)
			{

			}

			void configureXAxis(QtCharts::QCategoryAxis* xAxis) override
			{
				xAxis->setTitleText("Working Set Size");
			}

			void configureYAxis(QtCharts::QValueAxis* yAxis) override
			{
				yAxis->setTitleText("Bandwidth");
				yAxis->setLabelFormat("%.0f GB/s");
			}

			void configureChart(QtCharts::QChart* chart) override
			{
				chart->setTitle("Memory Read Bandwidth");
			}

			typename Elpida::MemoryTasksPropertiesWithChart<T>::ChartValues getChartValuesFromTaskThroughput(const TaskThroughput& throughput) override
			{
				return
				{
					throughput.getRatePerSecond() / (double)std::giga::num,
					ValueUtilities::getValueScaleString(throughput.getRunResult().getTestedDataValue())
				};
			}
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_UI_MEMORYBANDWIDTHCHART_HPP_ */
