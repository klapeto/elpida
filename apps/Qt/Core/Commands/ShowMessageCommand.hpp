//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_SHOWMESSAGECOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_SHOWMESSAGECOMMAND_HPP


#include <string>
#include <utility>
#include "Core/Abstractions/TypedCommand.hpp"

namespace Elpida
{
	class ShowMessageCommand final : public TypedCommand<ShowMessageCommand>
	{
	public:
		enum class Type
		{
			Info,
			Warning,
			Error
		};
		[[nodiscard]] const std::string& getMessage() const
		{
			return _message;
		}
		[[nodiscard]] Type getType() const
		{
			return _type;
		}

		ShowMessageCommand() = delete;
		explicit ShowMessageCommand(std::string message, Type type)
			: _message(std::move(message)), _type(type)
		{
		}
		~ShowMessageCommand() override = default;
	private:
		std::string _message;
		Type _type;
	};
}


#endif //APPS_QT_CORE_COMMANDS_SHOWMESSAGECOMMAND_HPP
