/**************************************************************************
*   Elpida -  Benchmark library
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
 * Help.cpp
 *
 *  Created on: 1 Ιουλ 2018
 *      Author: klapeto
 */

#include "Help.hpp"
#include "CommandRegistry.hpp"
#include <iostream>

namespace Elpida
{
	namespace CLI
	{
		Help::Help()
				: InterpreterCommand("help")
		{
			_description = "Displays the general help or help for specific commands.";
			_help = "Usage: help [command] [command]...";
		}

		Help::~Help()
		{
		}

		void Help::execute(Interpreter& executor, const std::vector<std::string>& arguments, const CommandRegistry& commandRegistry) const
		{
			size_t argsSize = arguments.size();
			if (argsSize >= 2)
			{
				for (size_t i = 1; i < argsSize; ++i)
				{
					const InterpreterCommand& command = commandRegistry.getCommand(arguments[i]);
					std::cout << command.getName() << ": " << command.getDescription() << std::endl << command.getHelp() << std::endl;
				}
			}
			else
			{
				printGeneralHelp();
			}
		}

		void Help::printGeneralHelp() const
		{
		}

	} /* namespace CLI */
} /* namespace Elpida */

