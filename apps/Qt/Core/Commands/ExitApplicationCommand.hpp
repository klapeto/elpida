//
// Created by klapeto on 9/4/20.
//

#ifndef EXITAPPLICATIONCOMMAND_HPP
#define EXITAPPLICATIONCOMMAND_HPP

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


#endif //EXITAPPLICATIONCOMMAND_HPP
