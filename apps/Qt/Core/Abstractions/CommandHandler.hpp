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
		virtual void handle(Command& command)
		{
		};
		virtual void handle(ShowLogsDialogCommand& command)
		{
		};
		virtual void handle(ShowAboutDialogCommand& command)
		{
		};
		virtual void handle(ExitApplicationCommand& command)
		{
		};
		virtual void handle(StartBenchmarkingCommand& command)
		{
		};
		virtual void handle(StopBenchmarkingCommand& command)
		{
		};
		virtual void handle(GetBenchmarksToRunCommand& command)
		{
		};
		virtual void handle(ShowMessageCommand& command)
		{
		};
		virtual void handle(GetTaskAffinityCommand& command)
		{
		};
		virtual void handle(SelectedBenchmarkChangedEvent& command)
		{
		};

		virtual ~CommandHandler() = default;
	};

}

#endif //APPS_QT_CORE_ABSTRACTIONS_COMMANDHANDLER_HPP
