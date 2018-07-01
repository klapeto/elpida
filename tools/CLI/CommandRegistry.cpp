/**************************************************************************
*   Elpida - Benchmark library
*   
*   Copyright (C) 2018  Ioannis Panagiotopoulos
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

/*
 * CommandRegistry.cpp
 *
 *  Created on: 1 Ιουλ 2018
 *      Author: klapeto
 */

#include "CommandRegistry.hpp"
#include "InterpreterCommand.hpp"
#include "Commands/Quit.hpp"
#include "Commands/Cpuid.hpp"
#include "Commands/Help.hpp"

namespace Elpida
{
	namespace CLI
	{
		CommandRegistry::CommandRegistry()
		{
			populateRegistry();
		}

		CommandRegistry::~CommandRegistry()
		{
			for (auto command : _commands)
			{
				delete command;
			}
		}

		const InterpreterCommand& CommandRegistry::getCommand(const std::string& commandName) const
		{
			auto itr = _commandsMap.find(commandName);
			if (itr != _commandsMap.end())
			{
				return *itr->second;
			}
			else
			{
				throw CommandNotFoundException();
			}
		}

		void CommandRegistry::addCommand(InterpreterCommand* command)
		{
			if (command != nullptr)
			{
				_commandsMap[command->getName()] = command;
				_commands.push_back(command);
			}
		}

		void CommandRegistry::populateRegistry()
		{
			addCommand(new Quit(), "exit");
			addCommand(new Cpuid());
			addCommand(new Help());
		}

	} /* namespace CLI */
} /* namespace Elpida */

