//
// Created by klapeto on 8/4/20.
//

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

#include "CommandForwardDeclarations.hpp"

namespace Elpida
{
	class CommandHandler
	{
	public:
		virtual void handle(const Command& command)
		{
		}

		virtual void handle(const ShowLogsDialogCommand& command)
		{
		}

		virtual void handle(const ShowAboutDialogCommand& command)
		{
		}

		virtual ~CommandHandler()
		{
		}
	};

}

#endif //COMMANDHANDLER_HPP
