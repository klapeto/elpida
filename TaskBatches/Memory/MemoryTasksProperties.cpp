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

#include "TaskBatches/Memory/MemoryTasksProperties.hpp"

#include "Elpida/Types/List.hpp"

#include <QtCharts/QChartView>
#include <QtCharts/QLineSeries>
#include <QtCharts/QLogValueAxis>
#include <QtCharts/QValueAxis>

namespace Elpida
{
	MemoryTasksProperties::MemoryTasksProperties()
			: QtTaskBatchWrapper(false, true), _chart(new QtCharts::QChart())
	{
		_chart->setTitle("Memory Read Bandwidth");
		_xAxis = new QtCharts::QLogValueAxis();
		_xAxis->setTitleText("Working Set Size");
		_xAxis->setLabelFormat("%.0f B");
		_xAxis->setBase(8);
		_xAxis->setMinorTickCount(-1);
		_chart->addAxis(_xAxis, Qt::AlignBottom);

		_yAxis = new QtCharts::QValueAxis();
		_yAxis->setTitleText("Bandwidth");
		_yAxis->setLabelFormat("%.0f GB/s");
		_chart->addAxis(_yAxis, Qt::AlignLeft);
	}

	MemoryTasksProperties::~MemoryTasksProperties()
	{
		delete _chart;
		delete _xAxis;
		delete _yAxis;
	}

	void MemoryTasksProperties::updateResultsChartData(const Map<String, Array<TaskThroughput>>& results)
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

		for (auto& result : ordered)
		{
			series->append(result->getRunResult().getOriginalValue(), result->getRatePerSecond() /  (double)std::giga::num);
		}
		_chart->addSeries(series);
		series->attachAxis(_yAxis);
		series->attachAxis(_xAxis);
		_yAxis->setMin(0);
	}

} /* namespace Elpida */

