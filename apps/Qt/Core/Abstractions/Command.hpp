//
// Created by klapeto on 8/4/20.
//

#ifndef COMMAND_HPP
#define COMMAND_HPP

namespace Elpida
{
	class CommandHandler;
	class Command
	{
	public:
		virtual void accept(CommandHandler& handler) const = 0;
		virtual ~Command()
		{

		}
	};
}


#endif //COMMAND_HPP
