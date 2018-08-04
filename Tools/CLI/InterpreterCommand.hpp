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

#include "Elpida/Types/Map.hpp"
#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"

namespace Elpida
{
	class CommandParser;

	namespace CLI
	{

		class Interpreter;
		class CommandRegistry;

		class InterpreterCommand
		{
			public:

				const String& getDescription() const
				{
					return _description;
				}

				const String& getName() const
				{
					return _name;
				}

				const String& getHelp() const
				{
					return _help;
				}

				const Array<String>& getAliases() const
				{
					return _aliases;
				}

				const String& getUsage() const
				{
					return _usage;
				}

				virtual void execute(Interpreter& executor, const CommandParser& commandParser, const CommandRegistry& commandRegistry) const=0;

				InterpreterCommand(const String& name)
						: _name(name)
				{

				}
				virtual ~InterpreterCommand()
				{

				}

			protected:
				Array<String> _aliases;
			private:
				String _name;
			protected:
				String _help;
				String _usage;
				String _description;
		};

	} /* namespace CLI */
} /* namespace Elpida */

#endif /* TOOLS_CLI_INTERPRETERCOMMAND_HPP_ */
