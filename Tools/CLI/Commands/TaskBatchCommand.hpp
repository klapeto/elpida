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
