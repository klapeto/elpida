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

#include "TaskBatches/Memory/Ui/MemoryTasksPropertiesWithChart.hpp"

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

		_xAxis = new QtCharts::QCategoryAxis();
		_chart->addAxis(_xAxis, Qt::AlignBottom);

		_yAxis = new QtCharts::QValueAxis();
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

		configureChart(_chart);
		configureXAxis(_xAxis);
		configureYAxis(_yAxis);

		auto series = new QtCharts::QLineSeries();
		auto ordered = List<const TaskThroughput*>();

		for (auto& result : results)
		{
			ordered.push_back(&result.second.at(0));
		}
		ordered.sort([](const TaskThroughput* a, const TaskThroughput* b)
		{
			return a->getRunResult().getTestedDataValue() < b->getRunResult().getTestedDataValue();
		});

		_xAxis->setLabelsPosition(QtCharts::QCategoryAxis::AxisLabelsPositionOnValue);
		_xAxis->setMax(ordered.size() - 1);
		_xAxis->setMin(0);
		auto c = 0;
		for (auto result : ordered)
		{
			auto values = getChartValuesFromTaskThroughput(*result);
			series->append(c, values.yValue);
			_xAxis->append(QString::fromStdString(values.xCategory), c++);
		}

		_chart->addSeries(series);
		series->attachAxis(_yAxis);
		series->attachAxis(_xAxis);
		_yAxis->setMin(0);
	}

} /* namespace Elpida */

