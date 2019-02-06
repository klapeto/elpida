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

#include "ElpidaManager.hpp"
#include <Elpida/Utilities/Logger.hpp>
#include <Elpida/TaskBatch.hpp>
#include <TaskBatches/QtTaskBatchWrapper.hpp>

constexpr const char* PageCreationFunctionName = "createQtBatchWrapper";

namespace Elpida
{
	ElpidaManager::ElpidaManager()
	{
		Logger::getInstance().setOutput(_log);
	}

	ElpidaManager::~ElpidaManager()
	{
		_log.flush();
		destroyTaskBatches();
	}

	void ElpidaManager::destroyTaskBatches()
	{
		for (auto& pair : _createdTaskBatches)
		{
			if (pair.second != nullptr)
			{
				delete pair.second;
			}
		}
		_createdTaskBatches.clear();
	}

	void ElpidaManager::reloadTaskBatches()
	{
		destroyTaskBatches();
		_batchLoader.loadFromFolder(_batchesDirectory, _batchesOrderFile);
		auto& loaded = _batchLoader.getLoadedPlugins();
		for (auto& plugin : loaded)
		{
			auto func = plugin.second.getFunctionPointer<Elpida::QtTaskBatchWrapper* (*)()>(PageCreationFunctionName);
			if (func != nullptr)
			{
				auto prop = func();
				auto& taskBatch = prop->getTaskBatch();
				_createdTaskBatches.emplace(taskBatch.getName(), prop);
			}
		}
	}
}
