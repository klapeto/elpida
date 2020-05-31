/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

//
// Created by klapeto on 18/4/20.
//

#ifndef APPS_QT_CORE_COMMANDS_SHOWMESSAGECOMMAND_HPP
#define APPS_QT_CORE_COMMANDS_SHOWMESSAGECOMMAND_HPP


#include <string>
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
