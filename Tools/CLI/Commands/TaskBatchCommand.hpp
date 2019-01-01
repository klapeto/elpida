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
 * TaskBatchCommand.hpp
 *
 *  Created on: 29 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef TOOLS_CLI_COMMANDS_TASKBATCHCOMMAND_HPP_
#define TOOLS_CLI_COMMANDS_TASKBATCHCOMMAND_HPP_

#include "InterpreterCommand.hpp"

namespace Elpida
{
	class TaskBatch;
	namespace CLI
	{
		class TaskBatchCommand final: public InterpreterCommand
		{
			public:

				void execute(Interpreter& executor, const CommandParser& commandParser, const CommandRegistry& commandRegistry) const;

				TaskBatchCommand(TaskBatch& taskBatch);
				~TaskBatchCommand();

			private:
				TaskBatch& _taskBatch;
		};

	} /* namespace CLI */
} /* namespace Elpida */

#endif /* TOOLS_CLI_COMMANDS_TASKBATCHCOMMAND_HPP_ */
