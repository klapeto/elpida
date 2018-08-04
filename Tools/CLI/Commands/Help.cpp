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

#include "Commands/Help.hpp"

#include <iostream>

#include "CommandRegistry.hpp"
#include "Elpida/Utilities/CommandParser.hpp"

namespace Elpida
{
	namespace CLI
	{
		Help::Help()
				: InterpreterCommand("help")
		{
			_description = "Displays the general help or help for specific commands.";
			_usage = "help [command]";
		}

		Help::~Help()
		{
		}

		void Help::execute(Interpreter& executor, const CommandParser& commandParser, const CommandRegistry& commandRegistry) const
		{
			auto unamedArguments = commandParser.getUnamedArguments();
			if (unamedArguments.size() == 2)
			{
				const InterpreterCommand& command = commandRegistry.getCommand(unamedArguments[1]);

				std::cout << command.getName() << ": " << command.getDescription() << std::endl << "Usage: " << command.getUsage()
				          << std::endl;
				{
					auto aliases = command.getAliases();
					if (aliases.size() > 0)
					{
						std::cout << "Aliases: ";
						for (size_t i = 0; i < aliases.size(); ++i)
						{
							std::cout << aliases[i];
							if (i < aliases.size() - 1)
							{
								std::cout << ", ";
							}
						}
						std::cout << std::endl;
					}
				}
				if (command.getHelp().size() > 0)
				{
					std::cout << command.getHelp();
				}
				std::cout << std::endl;
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

