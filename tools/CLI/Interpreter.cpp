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
 * Interpreter.cpp
 *
 *  Created on: 1 Ιουλ 2018
 *      Author: klapeto
 */

#include "Interpreter.hpp"
#include "InterpreterCommand.hpp"
#include "CommandRegistry.hpp"
#include "CommandNotFoundException.hpp"
#include "BadCommandException.hpp"
#include <iostream>

namespace Elpida
{
	namespace CLI
	{

		Interpreter::Interpreter(const CommandRegistry& commands)
				: _commands(commands), _stopRunning(false)
		{

		}

		void Interpreter::run()
		{
			std::cout << std::endl;
			std::string input;
			std::vector<std::string> arguments;
			while (!_stopRunning)
			{
				std::cout << "-> ";
				getline(std::cin, input);
				arguments.clear();
				parseCommand(input, arguments);
				if (arguments.size() > 0)
				{
					const std::string& commandName = arguments[0];
					try
					{
						const InterpreterCommand& command = _commands.getCommand(commandName);
						command.execute(*this, arguments, _commands);
					}
					catch (const CommandNotFoundException& e)
					{
						std::cout << '\'' << commandName << '\'' << " command does not exist." << std::endl;
					}
					catch (const BadCommandException& e) {
						std::cout << "[Error]: " << '\'' << e.getWhatThrewThis() << '\'' << " -> " << e.getMessage() << std::endl;
					}
				}
			}
		}

		Interpreter::~Interpreter()
		{

		}

		void Interpreter::parseCommand(const std::string& input, std::vector<std::string>& accumulator)
		{
			const char* ptr = input.c_str();
			const char* c = ptr;
			while (*ptr)
			{
				while (*(ptr++) == ' ')
					c++;
				while (*ptr != ' ' && *ptr)
					ptr++;
				accumulator.push_back(std::string(c, ptr));
				c = ptr;
			}
		}

	}  // namespace CLI
} /* namespace Elpida */

