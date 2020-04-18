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

#include "ui_MainWindow.h"
#include "MainWindow.hpp"

#include "Core/Commands/ExitApplicationCommand.hpp"
#include "Core/Commands/ShowAboutDialogCommand.hpp"
#include "Core/Commands/ShowLogsDialogCommand.hpp"
#include "Core/Abstractions/Mediator.hpp"

namespace Elpida
{

	MainWindow::MainWindow(Mediator& mediator)
		: QMainWindow(), _mediator(mediator), _ui(new Ui::MainWindow)
	{
		_ui->setupUi(this);
	}

	void MainWindow::addTab(QWidget& widget, const std::string& name)
	{
		_ui->tbwMain->addTab(&widget, QString::fromStdString(name));
	}

	MainWindow::~MainWindow()
	{
		delete _ui;
	}

	void MainWindow::on_actionExit_triggered()
	{
		_mediator.execute(ExitApplicationCommand());
	}

	void MainWindow::on_actionAbout_triggered()
	{
		_mediator.execute(ShowAboutDialogCommand());
	}

	void Elpida::MainWindow::on_actionShowLogs_triggered()
	{
		_mediator.execute(ShowLogsDialogCommand());
	}

}  // namespace Elpida
