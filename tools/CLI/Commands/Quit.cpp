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
 * Quit.cpp
 *
 *  Created on: 1 Ιουλ 2018
 *      Author: klapeto
 */

#include "../Commands/Quit.hpp"
#include "../Interpreter.hpp"

namespace Elpida
{
	namespace CLI
	{
		void Quit::execute(Interpreter& executor, const std::vector<std::string>& arguments, const CommandRegistry& commandRegistry) const
		{
			executor.stopRunning();
		}

		Quit::Quit(): InterpreterCommand("quit")
		{
			_description =  "Terminates the command-line input.";
			_help = "Usage: quit\n"
					"Aliases: exit\n"
					"After invoking the quit command, the command line input will\n"
					"stop receiving commands. If no other operation is after the\n"
					"interpreter, then the program terminates.\n";
		}

		Quit::~Quit()
		{
		}

	} /* namespace CLI */
} /* namespace Elpida */

