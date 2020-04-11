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
 * Task.cpp
 *
 *  Created on: 17 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/Task.hpp"

#include <utility>

#include "Elpida/TaskThread.hpp"
#include "Elpida/Topology/ProcessorNode.hpp"

namespace Elpida
{
	Task::Task(std::string name, bool toBeMeasured)
		: _name(std::move(name)), _toBeMeasured(toBeMeasured)
	{

	}

	void Task::applyAffinity()
	{
		auto nodes = _affinity.getProcessorNodes();
		if (nodes.size() > 0)
		{
			auto id = nodes[0]->getOsIndex();
			if (id >= 0)
			{
				TaskThread::setCurrentThreadAffinity((int)id);
			}
		}
	}

	void Task::addResult(const TaskRunResult& result)
	{
		_lastRunResults.push_back(&result);
	}

} /* namespace Elpida */

