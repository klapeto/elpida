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

#include "ElpidaManager.hpp"
#include <Elpida/Types/Map.hpp>
#include <Elpida/Types/Array.hpp>
#include <Elpida/OffThreadExecutor.hpp>
#include <QMainWindow>

class QTreeWidgetItem;

namespace Elpida
{
	class QtTaskBatchWrapper;
	class TaskBatchProperties;
	class RunTaskBatchDialog;

	namespace Ui
	{
		class MainWindow;
	}  // namespace Ui


	class MainWindow final: public QMainWindow
	{
		Q_OBJECT

		public:
			explicit MainWindow(QWidget *parent = 0);
			~MainWindow();

		protected:
			virtual void showEvent(QShowEvent *event) override;

		private slots:
			void on_actionExit_triggered();
			void on_actionAbout_triggered();

			void on_actionRunBatches_triggered();

		private:
			ElpidaManager _elpidaManager;
			Array<QMetaObject::Connection> _connections;
			Map<String, QtTaskBatchWrapper*> _createdPropetyPages;
			RunTaskBatchDialog *_runTaskBatchDialog;
			Ui::MainWindow *_ui;
			TaskBatchProperties* _taskBatchPropertiesDialog;
			OffThreadExecutor _offThreadExecutor;
			bool _fixedSizeSet;

			void loadCpuInfo();
			void loadTaskInfo();

			void onTwTaskBatchListDoubleClick(QTreeWidgetItem* item, int col);
			void addMascot();
	};

}  // namespace Elpida

#endif // MAINWINDOW_HPP
