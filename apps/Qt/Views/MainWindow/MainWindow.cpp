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


#include "ui_MainWindow.h"
#include "MainWindow.hpp"

#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"
#include "Models/Screens/ScreensModel.hpp"

#include <QMessageBox>

namespace Elpida
{

	MainWindow::MainWindow(Mediator& mediator, ScreensModel& screensModel)
		: QMainWindow(), _navigationBarView(screensModel), _mediator(mediator), _ui(new Ui::MainWindow),
		  _screensModel(screensModel)
	{
		_ui->setupUi(this);

		for (auto& itm: screensModel.getItems())
		{
			onScreenAdded(&itm.getValue());
		}

		addSubscription(_screensModel.itemAdded.subscribe([this](const CollectionChangedEventArgs<ScreenItem>& item)
		{
			emit screensModelItemAdded(&item.getItem().getValue());
		}));

		addSubscription(_screensModel.selectionChanged.subscribe([this](ScreenItem& item)
		{
			emit screensModelSelectedItemChanged(&item);
		}));

		QWidget::connect(this, &MainWindow::showMessageRequested, this, &MainWindow::showMessageRequestedHandler);
		QWidget::connect(this, &MainWindow::screensModelItemAdded, this, &MainWindow::onScreenAdded);
		QWidget::connect(this,
			&MainWindow::screensModelSelectedItemChanged, this, &MainWindow::onSelectedScreenChanged);

		_ui->wNavBar->layout()->addWidget(&_navigationBarView);
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

	void MainWindow::onScreenAdded(const ScreenItem* screen)
	{
		auto index = _ui->swPages->count();
		_ui->swPages->addWidget(screen->getWidget());
		_screensMaps.emplace(screen, index);
	}

	void MainWindow::onSelectedScreenChanged(const ScreenItem* screen)
	{
		auto index = _screensMaps.at(screen);
		_ui->swPages->setCurrentIndex(index);
	}

}  // namespace Elpida
