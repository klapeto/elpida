//
// Created by klapeto on 9/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_SHOWLOGSDIALOGCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_SHOWLOGSDIALOGCOMMAND_HPP

#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class ShowLogsDialogCommand final : public TypedCommand<ShowLogsDialogCommand>
	{
	public:
		constexpr ShowLogsDialogCommand() = default;
		~ShowLogsDialogCommand() override = default;
	};
}


#endif //APPS_QT_CORE_COMMANDS_SHOWLOGSDIALOGCOMMAND_HPP
