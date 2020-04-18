//
// Created by klapeto on 8/4/20.
//

#include "ElpidaMediator.hpp"

#include "Core/Abstractions/Command.hpp"

namespace Elpida
{
	void ElpidaMediator::execute(Command& command)
	{
		for (auto handler : _handlers)
		{
			command.accept(*handler);
		}
	}

	void ElpidaMediator::registerCommandHandler(CommandHandler& handler)
	{
		_handlers.push_back(&handler);
	}

	void ElpidaMediator::execute(Command&& command)
	{
		for (auto handler : _handlers)
		{
			command.accept(*handler);
		}
	}
}