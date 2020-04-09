//
// Created by klapeto on 8/4/20.
//

#ifndef APPS_QT_CORE_ABSTRACTIONS_COMMAND_HPP
#define APPS_QT_CORE_ABSTRACTIONS_COMMAND_HPP

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


#endif //APPS_QT_CORE_ABSTRACTIONS_COMMAND_HPP
