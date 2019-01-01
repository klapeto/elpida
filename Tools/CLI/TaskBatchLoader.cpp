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
 * TaskBatchLoader.cpp
 *
 *  Created on: 10 Οκτ 2018
 *      Author: klapeto
 */

#include "Tools/CLI/TaskBatchLoader.hpp"
#include <Elpida/Exceptions/ElpidaException.hpp>
#include <Elpida/TaskBatch.hpp>
#include <iostream>

namespace Elpida
{
	namespace CLI
	{

		TaskBatch& TaskBatchLoader::getBatch(const String& name)
		{
			auto itr = _loadedObjects.find(name);
			if (itr != _loadedObjects.end())
			{
				return *itr->second;
			}
			else
			{
				throw ElpidaException("TaskBatchLoader", "'" + name + "' batch was not found or not loaded.");
			}
		}

		void TaskBatchLoader::loadFromFolder(const String& path, const String& orderFile)
		{
			SharedLibraryLoader::loadFromFolder(path, orderFile);
			try
			{
				for (auto& library : _loadedLibraries)
				{
					auto taskBatchCreator = library.second.getFunctionPointer<TaskBatch* (*)()>("createTaskBatch");
					if (taskBatchCreator != nullptr)
					{
						TaskBatch* taskBatch = taskBatchCreator();
						_loadedObjects.emplace(taskBatch->getName(), taskBatch);
					}
					else
					{
						std::cerr << "Failed to get function 'createTaskBatch' from '" + path + "' plugin";
					}

				}
			}
			catch (ElpidaException& e)
			{
				std::cerr << "Failed to load '" + path + "' plugin: " + e.getMessage();
			}
		}
	} /* namespace CLI */
} /* namespace Elpida */
