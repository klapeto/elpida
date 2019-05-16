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
 * MemoryTasksProperties.cpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#include "TaskBatches/Memory/MemoryTasksPropertiesWithChart.hpp"

#include "Elpida/Types/List.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>
#include <QtCharts/QCategoryAxis>

namespace Elpida
{
	MemoryTasksPropertiesWithChart::MemoryTasksPropertiesWithChart(TaskBatch* taskBatch)
			: QtTaskBatchWrapper(false, true), _taskBatch(taskBatch), _chart(new QtCharts::QChart())
	{
		_chart->setTitle("Memory Read Bandwidth");
		_xAxis = new QtCharts::QCategoryAxis();
		_xAxis->setTitleText("Working Set Size");
		_chart->addAxis(_xAxis, Qt::AlignBottom);

		_yAxis = new QtCharts::QValueAxis();
		_yAxis->setTitleText("Bandwidth");
		_yAxis->setLabelFormat("%.0f GB/s");
		_chart->addAxis(_yAxis, Qt::AlignLeft);
	}

	MemoryTasksPropertiesWithChart::~MemoryTasksPropertiesWithChart()
	{
		delete _chart;
		delete _xAxis;
		delete _yAxis;
	}

	void MemoryTasksPropertiesWithChart::updateResultsChartData(const Map<String, Array<TaskThroughput>>& results)
	{
		_chart->removeAllSeries();
		auto series = new QtCharts::QLineSeries();
		auto ordered = List<const TaskThroughput*>();

		for (auto& result : results)
		{
			ordered.push_back(&result.second.at(0));
		}
		ordered.sort([](const TaskThroughput* a, const TaskThroughput* b)
		{
			return a->getRunResult().getOriginalValue() < b->getRunResult().getOriginalValue();
		});

		_xAxis->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
		_xAxis->setMax(ordered.size()-1);
		_xAxis->setMin(0);
		auto c = 0;
		for (auto& result : ordered)
		{
			series->append(c, result->getRatePerSecond() / (double)std::giga::num);
			_xAxis->append(QString::fromStdString(ValueUtilities::getValueScaleString(result->getRunResult().getOriginalValue())), c++);
		}

		_chart->addSeries(series);
		series->attachAxis(_yAxis);
		series->attachAxis(_xAxis);
		_yAxis->setMin(0);
	}

} /* namespace Elpida */

