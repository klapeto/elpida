//
// Created by klapeto on 8/4/20.
//

#include "ElpidaMediator.hpp"
#include "ElpidaCommandHandler.hpp"
#include "Core/Abstractions/Command.hpp"

namespace Elpida
{

	ElpidaMediator::ElpidaMediator(int& argC, char** argv)
		: _qApplication(argC, argv), _mainWindow(*this, _cpuInfo, _topology)
	{
		_logger.setOutput(_log);
	}

	ElpidaMediator::~ElpidaMediator()
	{

	}

	void ElpidaMediator::execute(const Command& command)
	{
		ElpidaCommandHandler handler(*this);
		command.accept(handler);
	}

	void ElpidaMediator::run()
	{
		_mainWindow.show();
		_qApplication.exec();
	}

}