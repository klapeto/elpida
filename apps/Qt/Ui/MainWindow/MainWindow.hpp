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

#ifndef APPS_QT_UI_MAINWINDOW_MAINWINDOW_HPP
#define APPS_QT_UI_MAINWINDOW_MAINWINDOW_HPP

#include <QMainWindow>

#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida
{
	class Mediator;

	namespace Ui
	{
		class MainWindow;
	}  // namespace Ui

	class MainWindow final : public QMainWindow, public CommandHandler
	{
	Q_OBJECT

	public:

		void handle(ShowMessageCommand &command) override;

		void addTab(QWidget& widget, const std::string& name);

		explicit MainWindow(Mediator& mediator);
		~MainWindow() override;
	private slots:
		void on_actionExit_triggered();
		void on_actionAbout_triggered();
		void on_actionShowLogs_triggered();
		static void showMessageRequestedHandler(const QString& message, int type);

		signals:
		void showMessageRequested(const QString& message, int type);

	private:
		Mediator& _mediator;
		Ui::MainWindow* _ui;
	};

}  // namespace Elpida

#endif //APPS_QT_UI_MAINWINDOW_MAINWINDOW_HPP
