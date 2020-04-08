//
// Created by klapeto on 8/4/20.
//

#ifndef ELPIDACOMMANDHANDLER_HPP
#define ELPIDACOMMANDHANDLER_HPP


#include "Core/Abstractions/CommandHandler.hpp"

namespace Elpida
{
	class ElpidaMediator;
	class ShowLogsDialogCommand;
	class ShowAboutDialogCommand;

	class ElpidaCommandHandler : public CommandHandler
	{
	public:
		void handle(const Command& command) override;
		void handle(const ShowLogsDialogCommand &command) override;
		void handle(const ShowAboutDialogCommand &command) override;

		ElpidaCommandHandler(ElpidaMediator& elpidaMediator);

		virtual ~ElpidaCommandHandler();
	private:
		ElpidaMediator& _elpidaMediator;
	};

}


#endif //ELPIDACOMMANDHANDLER_HPP
