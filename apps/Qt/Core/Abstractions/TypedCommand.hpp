//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_TYPEDCOMMAND_HPP
#define APPS_QT_CORE_ABSTRACTIONS_TYPEDCOMMAND_HPP

#include "Command.hpp"
#include "CommandHandler.hpp"
namespace Elpida
{

	template<typename T>
	class TypedCommand : public Command
	{
		void accept(CommandHandler& handler) override
		{
			handler.handle(static_cast<T&>(*this));
		}
	};
}


#endif //APPS_QT_CORE_ABSTRACTIONS_TYPEDCOMMAND_HPP
