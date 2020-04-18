//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_STOPBENCHMARKINGCOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_STOPBENCHMARKINGCOMMAND_HPP

#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class StopBenchmarkingCommand final : public TypedCommand<StopBenchmarkingCommand>
	{
	public:
		constexpr StopBenchmarkingCommand() = default;
		~StopBenchmarkingCommand() = default;
	};

}


#endif //APPS_QT_CORE_COMMANDS_STOPBENCHMARKINGCOMMAND_HPP
