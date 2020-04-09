//
// Created by klapeto on 9/4/20.
//

#ifndef SHOWABOUTDIALOGCOMMAND_HPP
#define SHOWABOUTDIALOGCOMMAND_HPP


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


#endif //SHOWABOUTDIALOGCOMMAND_HPP
