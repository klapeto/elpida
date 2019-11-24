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

	class MemoryBandwidthChart final: public MemoryTasksPropertiesWithChart
	{
		public:
			MemoryBandwidthChart(TaskBatch* taskBatch)
					: MemoryTasksPropertiesWithChart(taskBatch)
			{
			}
			~MemoryBandwidthChart()
			{
			}
		protected:
			void configureXAxis(QtCharts::QCategoryAxis* xAxis) override;
			void configureYAxis(QtCharts::QValueAxis* yAxis) override;
			void configureChart(QtCharts::QChart* chart) override;
			ChartValues getChartValuesFromTaskThroughput(const TaskThroughput& throughput) override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_UI_MEMORYBANDWIDTHCHART_HPP_ */
