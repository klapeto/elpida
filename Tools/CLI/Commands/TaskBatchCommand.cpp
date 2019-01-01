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
