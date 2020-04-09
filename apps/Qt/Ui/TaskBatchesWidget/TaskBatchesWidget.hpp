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

#ifndef APPS_QT_UI_TASKBATCHESWIDGET_TASKBATCHESWIDGET_HPP
#define APPS_QT_UI_TASKBATCHESWIDGET_TASKBATCHESWIDGET_HPP

#include <qobjectdefs.h>
#include <qstring.h>
#include <qtreewidget.h>
#include <string>
#include <unordered_map>
#include <vector>

#include "Elpida/OffThreadExecutor.hpp"
#include "Elpida/Runner.hpp"
#include "Elpida/TaskThroughput.hpp"
#include "TaskBatches/QtTaskBatchWrapper.hpp"

class QTreeWidgetItem;
namespace QtCharts
{
	class QChart;
	class QChartView;
}  // namespace QtCharts


namespace Elpida
{
	class QtTaskBatchWrapper;
	class TaskBatchProperties;
	class TaskAffinity;

	namespace Ui
	{
		class TaskBatchesWidget;
	}

	class TaskBatchesWidget final : public QWidget
	{
	Q_OBJECT
	public:
		explicit TaskBatchesWidget(const std::unordered_map<std::string, QtTaskBatchWrapper*>& taskBatchList,
			const TaskAffinity& affinity,
			QWidget* parent = nullptr);
		~TaskBatchesWidget();

	public slots:
		void onTaskBatchListModified();

	private:
		Runner _taskBatchRunner;
		OffThreadExecutor _taskRunnerThread;

		QString _runningText;
		QString _readyText;
		QString _na;
		const std::unordered_map<std::string, QtTaskBatchWrapper*>& _taskBatchList;
		Ui::TaskBatchesWidget* _ui;
		TaskBatchProperties* _taskBatchPropertiesDialog;
		QtCharts::QChart* _initialChart;
		const TaskAffinity& _affinity;

		std::unordered_map<QTreeWidgetItem*, std::unordered_map<std::string, std::vector<TaskThroughput>>>
			_cachedResults;

		void appendResults();
		void runTaskBatches();

	signals:
		void onSessionBegin();
		void onTaskBatchStart(const QString& name, int size);
		void onTaskStart(const QString& name);
		void onTaskEnd(const QString& name);
		void onTaskBatchEnd(const QString& name);
		void onSessionEnd();

	private slots:
		void on_pbRun_clicked();
		void on_pbStop_clicked();
		void on_twResults_itemClicked(QTreeWidgetItem* item, int column);
		void onListItemButtonClicked(const QString& name);
		void updateForSessionBegin();
		void updateForTaskBatchBegin(const QString& name, int size);
		void updateForTaskBegin(const QString& name);
		void updateForTaskEnd(const QString& name);
		void updateForTaskBatchEnd(const QString& name);
		void updateForSessionEnd();
	};

} // namespace Elpida

#endif // APPS_QT_UI_TASKBATCHESWIDGET_TASKBATCHESWIDGET_HPP
