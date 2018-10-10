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

		void TaskBatchLoader::loadFromFolder(const String& path)
		{
			PluginLoader::loadFromFolder(path);
			try
			{
				for (auto& plugin : _loadedPlugins)
				{
					auto taskBatchCreator = plugin.second.getFunctionPointer<TaskBatch* (*)()>("createTaskBatch");
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
