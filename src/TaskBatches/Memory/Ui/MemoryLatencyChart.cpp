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
 * MemoryLatencyChart.cpp
 *
 *  Created on: 24 Νοε 2019
 *      Author: klapeto
 */

#include "TaskBatches/Memory/Ui/MemoryLatencyChart.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

namespace Elpida
{

	void MemoryLatencyChart::configureXAxis(QtCharts::QCategoryAxis* xAxis)
	{
		xAxis->setTitleText("Working Set Size");
		xAxis->setLabelFormat("%f B");
	}

	void MemoryLatencyChart::configureYAxis(QtCharts::QValueAxis* yAxis)
	{
		yAxis->setTitleText("Latency");
		yAxis->setLabelFormat("%.0f ns");
	}

	void MemoryLatencyChart::configureChart(QtCharts::QChart* chart)
	{
		chart->setTitle("Memory Read Latency");
	}

	MemoryLatencyChart::ChartValues MemoryLatencyChart::getChartValuesFromTaskThroughput(const TaskThroughput& throughput)
	{
		return
		{
			throughput.getUniversalValue(),
			ValueUtilities::getValueScaleString(throughput.getRunResult().getTestedDataValue())
		};
	}

} /* namespace Elpida */
