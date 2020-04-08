//
// Created by klapeto on 8/4/20.
//

#ifndef TYPEDCOMMAND_HPP
#define TYPEDCOMMAND_HPP

#include "Command.hpp"
#include "CommandHandler.hpp"
namespace Elpida
{

	template<typename T>
	class TypedCommand : public Command
	{
		virtual void accept(CommandHandler& handler) const override
		{
			handler.handle(static_cast<T&>(this));
		}
	};
}


#endif //TYPEDCOMMAND_HPP
