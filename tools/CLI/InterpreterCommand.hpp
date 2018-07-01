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
 * InterpreterCommand.hpp
 *
 *  Created on: 1 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef TOOLS_CLI_INTERPRETERCOMMAND_HPP_
#define TOOLS_CLI_INTERPRETERCOMMAND_HPP_

#include <string>
#include <vector>

namespace Elpida
{
	namespace CLI
	{

		class Interpreter;
		class CommandRegistry;

		class InterpreterCommand
		{
			public:

				const std::string& getDescription() const
				{
					return _description;
				}

				const std::string& getName() const
				{
					return _name;
				}

				const std::string& getHelp() const
				{
					return _help;
				}

				virtual void execute(Interpreter& executor, const std::vector<std::string>& arguments,
				                     const CommandRegistry& commandRegistry) const=0;

				InterpreterCommand(const std::string& name)
						: _name(name)
				{

				}
				virtual ~InterpreterCommand()
				{

				}

			private:
				std::string _name;
			protected:
				std::string _description;
				std::string _help;
		};

	} /* namespace CLI */
} /* namespace Elpida */

#endif /* TOOLS_CLI_INTERPRETERCOMMAND_HPP_ */
