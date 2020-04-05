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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <qmainwindow.h>
#include <qobjectdefs.h>
#include <vector>

#include "Elpida/OffThreadExecutor.hpp"

namespace Elpida
{
	class ElpidaManager;
} /* namespace Elpida */

class QTreeWidgetItem;

namespace Elpida
{
	class QtTaskBatchWrapper;
	class TaskBatchProperties;
	class TaskBatchesWidget;
	class LogsDialog;
	class TopologyWidget;

	namespace Ui
	{
		class MainWindow;
	}  // namespace Ui


	class MainWindow final: public QMainWindow
	{
		Q_OBJECT

		public:
			explicit MainWindow(ElpidaManager& elpidaManager, QWidget *parent = nullptr);
			~MainWindow();

		protected:
			virtual void showEvent(QShowEvent *event) override;

		private slots:
			void on_actionExit_triggered();
			void on_actionAbout_triggered();

			void on_actionShowLogs_triggered();

		private:
			ElpidaManager& _elpidaManager;
			std::vector<QMetaObject::Connection> _connections;
			TaskBatchesWidget* _taskBatchesWidget;
			LogsDialog* _logsDialog;
			TopologyWidget* _topologyWidget;
			Ui::MainWindow *_ui;
			OffThreadExecutor _offThreadExecutor;
			bool _fixedSizeSet;

			void loadCpuInfo();

			void addMascot();
	};

}  // namespace Elpida

#endif // MAINWINDOW_HPP
