/*** ***********************************************************************
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
 * MemoryLatencyChart.hpp
 *
 *  Created on: 24 Νοε 2019
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_UI_MEMORYLATENCYCHART_HPP_
#define TASKBATCHES_MEMORY_UI_MEMORYLATENCYCHART_HPP_

#include "TaskBatches/Memory/Ui/MemoryTasksPropertiesWithChart.hpp"
#include "TaskBatches/Memory/Latency/MemoryLatencyTaskBatch.hpp"

namespace Elpida
{

	class MemoryLatencyChart final : public MemoryTasksPropertiesWithChart<MemoryLatencyTaskBatch>
	{
	public:
		explicit MemoryLatencyChart(MemoryLatencyTaskBatch* taskBatch)
			: MemoryTasksPropertiesWithChart<MemoryLatencyTaskBatch>(taskBatch)
		{
		}
		~MemoryLatencyChart() override = default;
	protected:
		void configureXAxis(QtCharts::QCategoryAxis* xAxis) override;
		void configureYAxis(QtCharts::QValueAxis* yAxis) override;
		void configureChart(QtCharts::QChart* chart) override;
		ChartValues getChartValuesFromTaskThroughput(const TaskThroughput& throughput) override;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_UI_MEMORYLATENCYCHART_HPP_ */
