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
		virtual void handle(const Command& command)=0;
		virtual void handle(const ShowLogsDialogCommand& command)=0;
		virtual void handle(const ShowAboutDialogCommand& command)=0;
		virtual void handle(const ExitApplicationCommand& command)=0;

		virtual ~CommandHandler()
		{
		}
	};

}

#endif //COMMANDHANDLER_HPP
