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

#include <QtWidgets/QMessageBox>
#include "ui_MainWindow.h"
#include "MainWindow.hpp"

#include "Core/Commands/ShowLogsDialogCommand.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"

namespace Elpida
{

	MainWindow::MainWindow(Mediator& mediator)
		: QMainWindow(), _mediator(mediator), _ui(new Ui::MainWindow)
	{
		_ui->setupUi(this);
		QWidget::connect(this, &MainWindow::showMessageRequested, this, &MainWindow::showMessageRequestedHandler);
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
		QApplication::quit();
	}

	void MainWindow::on_actionAbout_triggered()
	{
		QMessageBox::about(
			QApplication::activeWindow(),
			"About: Elpida",
			"Elpida is an open source x86 Cpu/Algorithm benchmarking tool. It is released under the General Public License v3 (GPL v3). More info at: https://gitlab.com/dev-hood/elpida/elpida");

	}

	void Elpida::MainWindow::on_actionShowLogs_triggered()
	{
		_mediator.execute(ShowLogsDialogCommand());
	}

	void MainWindow::handle(ShowMessageCommand& command)
	{
		emit showMessageRequested(QString::fromStdString(command.getMessage()), static_cast<int>(command.getType()));
	}

	void MainWindow::showMessageRequestedHandler(const QString& message, int type)
	{
		switch (static_cast<ShowMessageCommand::Type>(type))
		{
		case ShowMessageCommand::Type::Info:
			QMessageBox::information(this, "Information", message);
			break;
		case ShowMessageCommand::Type::Warning:
			QMessageBox::warning(this, "Warning", message);
			break;
		case ShowMessageCommand::Type::Error:
			QMessageBox::critical(this, "Error", message);
			break;
		}
	}

}  // namespace Elpida
