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
 * ListBatches.cpp
 *
 *  Created on: 16 Ιουλ 2018
 *      Author: klapeto
 */

#include "Commands/ListBatches.hpp"

#include <iostream>

#include "TaskBatchLoader.hpp"
#include "Elpida/TaskBatch.hpp"

namespace Elpida
{
	namespace CLI
	{

		ListBatches::ListBatches(TaskBatchLoader& batchLoader)
				: InterpreterCommand("list"), _batchLoader(batchLoader)
		{
			_description = "List the available task batches to run.";
			_usage = "list";
		}

		ListBatches::~ListBatches()
		{
		}

		void ListBatches::execute(Interpreter& executor, const CommandParser& commandParser, const CommandRegistry& commandRegistry) const
		{
			auto batches = _batchLoader.getBatches();
			for (auto& batch : batches)
			{
				std::cout << batch.first << " -> " << batch.second->getName() << std::endl;
			}
		}

	} /* namespace CLI */
} /* namespace Elpida */
