//
// Created by klapeto on 18/4/20.
//

#include <QtWidgets/QMessageBox>
#include "CoreController.hpp"
#include "Core/Commands/ShowMessageCommand.hpp"

namespace Elpida
{

	CoreController::CoreController(int& argC, char** argV)
		: _qApplication(argC, argV)
	{

	}

	void CoreController::handle(ExitApplicationCommand& command)
	{
		QApplication::quit();
	}

	void CoreController::handle(ShowAboutDialogCommand& command)
	{
		QMessageBox::about(
			QApplication::activeWindow(),
			"About: Elpida",
			"Elpida is an open source x86 Cpu/Algorithm benchmarking tool. It is released under the General Public License v3 (GPL v3). More info at: https://gitlab.com/dev-hood/elpida/elpida");
	}

	void CoreController::run()
	{
		QApplication::exec();
	}

	void CoreController::handle(ShowMessageCommand& command)
	{
		switch (command.getType())
		{
		case ShowMessageCommand::Type::Info:
			QMessageBox::information(QApplication::activeWindow(),
				"Information",
				QString::fromStdString(command.getMessage()));
			break;
		case ShowMessageCommand::Type::Warning:
			QMessageBox::warning(QApplication::activeWindow(), "Warning", QString::fromStdString(command.getMessage()));
			break;
		case ShowMessageCommand::Type::Error:
			QMessageBox::critical(QApplication::activeWindow(), "Error", QString::fromStdString(command.getMessage()));
			break;
		}

	}
}