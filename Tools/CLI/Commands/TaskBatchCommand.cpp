/*
 * TaskBatchCommand.cpp
 *
 *  Created on: 29 Ιουλ 2018
 *      Author: klapeto
 */

#include "Commands/TaskBatchCommand.hpp"

#include <sstream>

#include "Elpida/Runner.hpp"
#include "Elpida/TaskBatch.hpp"
#include "Elpida/Utilities/CommandParser.hpp"

namespace Elpida
{
	namespace CLI
	{

		TaskBatchCommand::TaskBatchCommand(TaskBatch& taskBatch)
				: InterpreterCommand(taskBatch.getName()), _taskBatch(taskBatch)
		{
			_description = _taskBatch.getName();
			_usage = _taskBatch.getName() + " -arg1=value -arg2 ...";
			_help = "Runs this tasks batch with the provided arguments\n";
		}

		TaskBatchCommand::~TaskBatchCommand()
		{

		}

		void TaskBatchCommand::execute(Interpreter& executor, const CommandParser& commandParser,
		                               const CommandRegistry& commandRegistry) const
		{
			std::ostringstream argumentAccumulator;
			{
				auto namedArguments = commandParser.getNamedArguments();
				for (auto& arg : namedArguments)
				{
					if (arg.second.size() > 0)
					{
						argumentAccumulator << '-' << arg.first << '=' << arg.second << ' ';
					}
					else
					{
						argumentAccumulator << '-' << arg.first << ' ';
					}

				}
			}
			{
				auto unamedArguments = commandParser.getUnamedArguments();
				for (auto& arg : unamedArguments)
				{
					argumentAccumulator << arg << ' ';
				}
			}

			_taskBatch.reconfigure(argumentAccumulator.str());

			{
				Runner runner;
				runner.addTaskBatch(_taskBatch);

				runner.executeTasks();
			}
		}

	} /* namespace CLI */
} /* namespace Elpida */
