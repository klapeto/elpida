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
 * TaskBatch.cpp
 *
 *  Created on: 13 Μαρ 2018
 *      Author: klapeto
 */

#include "Elpida/TaskBatch.hpp"

#include <utility>

#include "Elpida/Task.hpp"

namespace Elpida
{
	TaskBatch::TaskBatch(std::string name)
		: _name(std::move(name))
	{

	}

	TaskBatch::~TaskBatch()
	{
		destroyTasks();
	}

	void TaskBatch::destroyTasks() const
	{
		for (auto task : _tasks)
		{
			delete task;
		}
		_tasks.clear();
	}

	void TaskBatch::prepare() const
	{
		destroyTasks();
		createTasks();
	}

	void TaskBatch::finalize() const
	{
		destroyTasks();
	}

	void TaskBatch::addTask(Task* task) const
	{
		_tasks.push_back(task);
	}

} /* namespace Elpida */
