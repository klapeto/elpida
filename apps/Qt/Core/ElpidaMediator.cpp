//
// Created by klapeto on 8/4/20.
//

#include <QtWidgets/QMessageBox>
#include "ElpidaMediator.hpp"
#include "Core/Abstractions/Command.hpp"

namespace Elpida
{

	ElpidaMediator::ElpidaMediator(int& argC, char** argv)
		: _qApplication(argC, argv), _mainWindow(*this), _systemInfoWidget(*this, _cpuInfo, _topology), _logsDialog(&_mainWindow, _logger)
	{
		_mainWindow.addTab(&_systemInfoWidget, "System Info");
	}

	ElpidaMediator::~ElpidaMediator()
	{

	}

	void ElpidaMediator::execute(const Command& command)
	{
		command.accept(*this);
	}

	void ElpidaMediator::run()
	{
		_mainWindow.show();
		_qApplication.exec();
	}

	void ElpidaMediator::handle(const Command& command)
	{

	}

	void ElpidaMediator::handle(const ShowLogsDialogCommand& command)
	{
		_logsDialog.exec();
	}

	void ElpidaMediator::handle(const ShowAboutDialogCommand& command)
	{
		QMessageBox::about(
			&_mainWindow,
			"About: Elpida",
			"Elpida is an open source x86 Cpu/Algorithm benchmarking tool. It is released under the General Public License v3 (GPL v3). More info at: https://github.com/klapeto/elpida");
	}

	void ElpidaMediator::handle(const ExitApplicationCommand& command)
	{
		_qApplication.quit();
	}
}