/*
 * MemoryBandwidthChart.cpp
 *
 *  Created on: 24 Νοε 2019
 *      Author: klapeto
 */

#include "TaskBatches/Memory/Ui/MemoryBandwidthChart.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

namespace Elpida
{
	void MemoryBandwidthChart::configureXAxis(QtCharts::QCategoryAxis* xAxis)
	{
		xAxis->setTitleText("Working Set Size");
	}

	void MemoryBandwidthChart::configureYAxis(QtCharts::QValueAxis* yAxis)
	{
		yAxis->setTitleText("Bandwidth");
		yAxis->setLabelFormat("%.0f GB/s");
	}

	void MemoryBandwidthChart::configureChart(QtCharts::QChart* chart)
	{
		chart->setTitle("Memory Read Bandwidth");
	}

	MemoryBandwidthChart::ChartValues MemoryBandwidthChart::getChartValuesFromTaskThroughput(const TaskThroughput& throughput)
	{
		return
		{
			throughput.getRatePerSecond() / (double)std::giga::num,
			ValueUtilities::getValueScaleString(throughput.getRunResult().getTestedDataValue())
		};
	}

} /* namespace Elpida */
