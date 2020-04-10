//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_COMMANDHANDLER_HPP
#define APPS_QT_CORE_ABSTRACTIONS_COMMANDHANDLER_HPP

#include "CommandForwardDeclarations.hpp"

namespace Elpida
{
	class CommandHandler
	{
	public:
		virtual void handle(const Command& command) = 0;
		virtual void handle(const ShowLogsDialogCommand& command) = 0;
		virtual void handle(const ShowAboutDialogCommand& command) = 0;
		virtual void handle(const ExitApplicationCommand& command) = 0;

		virtual ~CommandHandler() = default;
	};

}

#endif //APPS_QT_CORE_ABSTRACTIONS_COMMANDHANDLER_HPP
