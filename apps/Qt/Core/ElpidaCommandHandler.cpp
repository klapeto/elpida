//
// Created by klapeto on 8/4/20.
//

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
}

