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

#include <iostream>

#include "CommandRegistry.hpp"
#include "InterpreterCommand.hpp"
#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"
#include "BadCommandException.hpp"
#include "CommandNotFoundException.hpp"
#include "Elpida/Utilities/CommandParser.hpp"

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
			String input;
			CommandParser commandParser;
			while (!_stopRunning)
			{
				std::cout << "-> ";
				getline(std::cin, input);

				commandParser.parseCommand(input);
				auto unamedArguments = commandParser.getUnamedArguments();

				if (unamedArguments.size() > 0)
				{
					const String& commandName = unamedArguments[0];
					try
					{
						const InterpreterCommand& command = _commands.getCommand(commandName);
						command.execute(*this, commandParser, _commands);
					}
					catch (const CommandNotFoundException& e)
					{
						std::cout << '\'' << commandName << '\'' << " command does not exist." << std::endl;
					}
					catch (const BadCommandException& e)
					{
						std::cout << "[Bad Command]: " << '\'' << e.what() << '\'' << " -> " << e.getMessage() << std::endl;
					}
					catch (const ElpidaException& e)
					{
						std::cout << "[Error]: " << '\'' << e.what() << '\'' << " -> " << e.getMessage() << std::endl;
					}
				}
			}
		}

		Interpreter::~Interpreter()
		{

		}

	}  // namespace CLI
} /* namespace Elpida */

