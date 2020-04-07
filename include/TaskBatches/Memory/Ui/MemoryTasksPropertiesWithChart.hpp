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
 * MemoryTasksPropertiesWithChart.hpp
 *
 *  Created on: 18 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TASKBATCHES_MEMORY_MEMORYTASKSPROPERTIESWITHCHART_HPP_
#define TASKBATCHES_MEMORY_MEMORYTASKSPROPERTIESWITHCHART_HPP_

#include <qstring.h>
#include <QtCharts/qcategoryaxis.h>
#include <QtCharts/qchart.h>
#include <QtCharts/qlineseries.h>
#include <QtCharts/qvalueaxis.h>
#include <list>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "Elpida/TaskBatch.hpp"
#include "Elpida/TaskRunResult.hpp"
#include "Elpida/TaskThroughput.hpp"
#include "TaskBatches/QtTaskBatchWrapper.hpp"

namespace QtCharts
{
	class QChart;
	class QValueAxis;
	class QCategoryAxis;
}  // namespace QtCharts

namespace Elpida
{

	template<typename T>
	class MemoryTasksPropertiesWithChart : public QtTaskBatchWrapper
	{
	public:

		const T& getTaskBatch() const override
		{
			return *_taskBatch;
		}

		void reconfigureTaskBatch() override
		{

		}
		void validateConfiguration() override
		{

		}

		QtCharts::QChart* getResultsChartContainer() override
		{
			return _chart;
		}

		void updateResultsChartData(const std::unordered_map<std::string,
															 std::vector<TaskThroughput>>& results) override
		{
			_chart->removeAllSeries();

			configureChart(_chart);
			configureXAxis(_xAxis);
			configureYAxis(_yAxis);

			auto series = new QtCharts::QLineSeries();
			auto ordered = std::list<const TaskThroughput*>();

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

		MemoryTasksPropertiesWithChart(T* taskBatch)
			: MemoryTasksPropertiesWithChart(taskBatch, false)
		{

		}
		virtual ~MemoryTasksPropertiesWithChart()
		{
			delete _chart;
		}
	private:
		QtCharts::QChart* _chart;
		QtCharts::QCategoryAxis* _xAxis;
		QtCharts::QValueAxis* _yAxis;
	protected:
		T* _taskBatch;

		MemoryTasksPropertiesWithChart(T* taskBatch, bool hasProperties)
			: QtTaskBatchWrapper(hasProperties, true), _chart(new QtCharts::QChart()), _taskBatch(taskBatch)
		{
			_xAxis = new QtCharts::QCategoryAxis();
			_chart->addAxis(_xAxis, Qt::AlignBottom);

			_yAxis = new QtCharts::QValueAxis();
			_chart->addAxis(_yAxis, Qt::AlignLeft);
		}

		struct ChartValues
		{
			double yValue;
			std::string xCategory;
		};

		virtual void configureXAxis(QtCharts::QCategoryAxis* xAxis) = 0;
		virtual void configureYAxis(QtCharts::QValueAxis* yAxis) = 0;
		virtual void configureChart(QtCharts::QChart* chart) = 0;
		virtual ChartValues getChartValuesFromTaskThroughput(const TaskThroughput& throughput) = 0;
	};

} /* namespace Elpida */

#endif /* TASKBATCHES_MEMORY_MEMORYTASKSPROPERTIESWITHCHART_HPP_ */
