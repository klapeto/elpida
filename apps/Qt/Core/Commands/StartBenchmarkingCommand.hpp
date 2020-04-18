//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_STARTBENCHMARKINGCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_STARTBENCHMARKINGCOMMAND_HPP

#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class StartBenchmarkingCommand final : public TypedCommand<StartBenchmarkingCommand>
	{
	public:
		constexpr StartBenchmarkingCommand() = default;
		~StartBenchmarkingCommand() = default;
	};
}


#endif //APPS_QT_CORE_COMMANDS_STARTBENCHMARKINGCOMMAND_HPP
