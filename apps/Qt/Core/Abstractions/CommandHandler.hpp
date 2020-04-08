//
// Created by klapeto on 8/4/20.
//

#ifndef COMMANDHANDLER_HPP
#define COMMANDHANDLER_HPP

namespace Elpida
{
	class Command;
	class CommandHandler
	{
	public:
		virtual void handle(const Command& command)
		{

		}

		virtual ~CommandHandler()
		{

		}
	};

}

#endif //COMMANDHANDLER_HPP
