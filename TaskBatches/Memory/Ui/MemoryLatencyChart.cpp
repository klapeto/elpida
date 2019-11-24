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
