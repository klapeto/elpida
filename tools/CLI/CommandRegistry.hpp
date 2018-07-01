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
 * CommandRegistry.hpp
 *
 *  Created on: 1 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef TOOLS_CLI_COMMANDREGISTRY_HPP_
#define TOOLS_CLI_COMMANDREGISTRY_HPP_

#include <unordered_map>
#include <vector>
#include <string>

#include "CommandNotFoundException.hpp"

namespace Elpida
{
	namespace CLI
	{

		class InterpreterCommand;

		class CommandRegistry
		{
			public:

				const InterpreterCommand& getCommand(const std::string& commandName) const;

				CommandRegistry();
				virtual ~CommandRegistry();
			private:
				std::vector<InterpreterCommand*> _commands;
				std::unordered_map<std::string, InterpreterCommand*> _commandsMap;
			protected:

				template<typename T, typename ... Targs>
				void addCommand(InterpreterCommand* command, const T& currentName, const Targs&... restNames)
				{
					if (command != nullptr)
					{
						_commandsMap[currentName] = command;
						addCommand(command, restNames...);
					}

				}

				void addCommand(InterpreterCommand* command);
				virtual void populateRegistry();

		};

	} /* namespace CLI */
} /* namespace Elpida */

#endif /* TOOLS_CLI_COMMANDREGISTRY_HPP_ */
