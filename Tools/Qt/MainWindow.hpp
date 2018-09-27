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

#include <Elpida/TaskBatchLoader.hpp>
#include <Elpida/Types/Map.hpp>
#include <QMainWindow>

namespace Ui
{
	class MainWindow;

}
class TaskBatchProperties;
class QTreeWidgetItem;

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

	private:
		Elpida::TaskBatchLoader _tasksLoader;
		Elpida::Array<QMetaObject::Connection> _connections;
		Elpida::Map<Elpida::String, QWidget*> _createdPropetyPages;
		Ui::MainWindow *_ui;
		TaskBatchProperties* _taskBatchPropertiesDialog;
		bool _fixedSizeSet;

		void loadCpuInfo();
		void loadTaskInfo();

		void onTwTaskBatchListDoubleClick(QTreeWidgetItem* item, int col);
};

#endif // MAINWINDOW_HPP
