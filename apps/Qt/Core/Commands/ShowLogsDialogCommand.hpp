//
// Created by klapeto on 9/4/20.
//

#ifndef SHOWLOGSDIALOGCOMMAND_HPP
#define SHOWLOGSDIALOGCOMMAND_HPP

#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{

	class ShowLogsDialogCommand final : public TypedCommand<ShowLogsDialogCommand>
	{
	public:
		explicit ShowLogsDialogCommand()
		{

		}
		~ShowLogsDialogCommand()
		{

		}
	};
}


#endif //SHOWLOGSDIALOGCOMMAND_HPP
