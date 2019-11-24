/*
 * MemoryLatencyChart.hpp
 *
 *  Created on: 24 Νοε 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_UI_MEMORYLATENCYCHART_HPP_
#define TASKBATCHES_MEMORY_UI_MEMORYLATENCYCHART_HPP_

#include "TaskBatches/Memory/Ui/MemoryTasksPropertiesWithChart.hpp"

namespace Elpida
{

	class MemoryLatencyChart final: public MemoryTasksPropertiesWithChart
	{
		public:
			MemoryLatencyChart(TaskBatch* taskBatch)
					: MemoryTasksPropertiesWithChart(taskBatch)
			{
			}
			~MemoryLatencyChart()
			{

			}
		protected:
			void configureXAxis(QtCharts::QCategoryAxis* xAxis) override;
			void configureYAxis(QtCharts::QValueAxis* yAxis) override;
			void configureChart(QtCharts::QChart* chart) override;
			ChartValues getChartValuesFromTaskThroughput(const TaskThroughput& throughput) override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_UI_MEMORYLATENCYCHART_HPP_ */
