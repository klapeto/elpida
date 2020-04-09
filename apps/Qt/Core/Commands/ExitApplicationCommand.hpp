//
// Created by klapeto on 9/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_EXITAPPLICATIONCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_EXITAPPLICATIONCOMMAND_HPP

#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class ExitApplicationCommand final : public TypedCommand<ExitApplicationCommand>
	{
	public:
		explicit ExitApplicationCommand() = default;
		~ExitApplicationCommand() = default;
	};
}


#endif //APPS_QT_CORE_COMMANDS_EXITAPPLICATIONCOMMAND_HPP
