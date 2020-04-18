//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ELPIDAMEDIATOR_HPP
#define APPS_QT_CORE_ELPIDAMEDIATOR_HPP

#include "Core/Abstractions/Mediator.hpp"
#include <vector>

namespace Elpida
{
	class CommandHandler;

	class ElpidaMediator final : public Mediator
	{
	public:
		void execute(Command& command) override;
		void execute(Command&& command) override;

		void registerCommandHandler(CommandHandler& handler);

		ElpidaMediator() = default;
		~ElpidaMediator() override = default;
	private:
		std::vector<CommandHandler*> _handlers;

		void initializeSystemTopologyWidget();
		void initializeTaskBatchesTab();
	};
}


#endif //APPS_QT_CORE_ELPIDAMEDIATOR_HPP
