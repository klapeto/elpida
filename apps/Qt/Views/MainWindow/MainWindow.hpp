/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
#include <Elpida/EventsSubscriber.hpp>
#include "Views/NavigationBarView/NavigationBarView.hpp"

#include "Core/Abstractions/CommandHandler.hpp"

class QHBoxLayout;
class QPushButton;

namespace Elpida
{
	class Mediator;
	class ScreensModel;
	class ScreenItem;

	namespace Ui
	{
		class MainWindow;
	}  // namespace Views

	class MainWindow final : public QMainWindow, public CommandHandler, public EventsSubscriber
	{
	Q_OBJECT
	public:

		void handle(ShowMessageCommand& command) override;

		explicit MainWindow(Mediator& mediator, ScreensModel& screensModel);
		~MainWindow() override;
	private slots:
		void on_actionExit_triggered();
		void on_actionAbout_triggered();
		void showMessageRequestedHandler(const QString& message, int type);

		void onScreenAdded(const ScreenItem* screen);
		void onSelectedScreenChanged(const ScreenItem* screen);

	signals:
		void showMessageRequested(const QString& message, int type);
		void screensModelItemAdded(const ScreenItem* screen);
		void screensModelSelectedItemChanged(const ScreenItem* screen);

	private:
		std::unordered_map<const ScreenItem*, int> _screensMaps;
		NavigationBarView _navigationBarView;
		Mediator& _mediator;
		Ui::MainWindow* _ui;
		ScreensModel& _screensModel;

	};

}  // namespace Elpida

#endif //APPS_QT_UI_MAINWINDOW_MAINWINDOW_HPP
