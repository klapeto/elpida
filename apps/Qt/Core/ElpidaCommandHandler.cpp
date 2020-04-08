//
// Created by klapeto on 8/4/20.
//

#include <QtWidgets/QMessageBox>
#include "ElpidaCommandHandler.hpp"
#include "ElpidaMediator.hpp"

namespace Elpida
{
	ElpidaCommandHandler::ElpidaCommandHandler(ElpidaMediator& elpidaMediator)
		: _elpidaMediator(elpidaMediator)
	{

	}

	ElpidaCommandHandler::~ElpidaCommandHandler()
	{

	}

	void ElpidaCommandHandler::handle(const Command& command)
	{

	}

	void ElpidaCommandHandler::handle(const ShowLogsDialogCommand& command)
	{
		QMessageBox::about(
			&_elpidaMediator._mainWindow,
			"About: Elpida",
			"Elpida is an open source x86 Cpu/Algorithm benchmarking tool. It is released under the General Public License v3 (GPL v3). More info at: https://github.com/klapeto/elpida");
	}

	void ElpidaCommandHandler::handle(const ShowAboutDialogCommand& command)
	{

	}
}

