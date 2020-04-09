//
// Created by klapeto on 9/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_SHOWABOUTDIALOGCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_SHOWABOUTDIALOGCOMMAND_HPP


#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class ShowAboutDialogCommand final : public TypedCommand<ShowAboutDialogCommand>
	{
	public:
		explicit ShowAboutDialogCommand() = default;
		~ShowAboutDialogCommand() = default;
	};
}


#endif //APPS_QT_CORE_COMMANDS_SHOWABOUTDIALOGCOMMAND_HPP
