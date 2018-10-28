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
 * PluginLoader.cpp
 *
 *  Created on: 2 Ιουλ 2018
 *      Author: klapeto
 */

#include <iostream>

#include "Config.hpp"
#include "Elpida/TaskBatch.hpp"
#include "Elpida/Exceptions/ElpidaException.hpp"
#include "Elpida/PluginLoader.hpp"
#include "Elpida/Utilities/FileSystem.hpp"
#include "Elpida/Utilities/Logger.hpp"
#include <fstream>
#include <algorithm>

#if _elpida_linux
#define TASK_ENDING ".so"
#elif _elpida_windows
#define TASK_ENDING ".dll"
#endif

#define LOAD_ORDER_FILE "plugins"

namespace Elpida
{

	PluginLoader::PluginLoader()
	{

	}

	void PluginLoader::loadFromFolder(const String& path)
	{
		unloadEverything();

		Array<String> loadFilenames;

		FileSystem::iterateDirectory(path, [&loadFilenames](const String& filePath)
		{
			if (filePath.find(TASK_ENDING) != std::string::npos)
			{
				loadFilenames.push_back(filePath);
			}
		});

		std::ifstream orderFile(path + PATH_SEPERATOR + LOAD_ORDER_FILE);
		if (orderFile.good())
		{
			String line;
			auto pred = [&line](const String& val){	return val.find(line + TASK_ENDING) != String::npos;};
			while (std::getline(orderFile, line))
			{
				auto itr = std::find_if(loadFilenames.begin(), loadFilenames.end(), pred);
				if (itr != loadFilenames.end())
				{
					if (FileSystem::fileExists(*itr))
					{
						loadPlugin(*itr);
					}
					else
					{
						Logger::getInstance().log(Logger::LogType::Warning, "Failed to open referenced plugin '", line, '\'');
					}
				}
				else
				{
					Logger::getInstance().log(Logger::LogType::Warning, "A plugin referenced on 'plugins' file was not found: ", line);
				}
			}
			orderFile.close();
		}
		else
		{
			Logger::getInstance().log(Logger::LogType::Error,"'", LOAD_ORDER_FILE, "' file inside '", path, "' was not found. Elpida does not know which plugins to load.");
		}
	}

	PluginLoader::~PluginLoader()
	{

	}

	void PluginLoader::loadPlugin(const String& path)
	{
		try
		{
			Plugin plugin = Plugin(path);
			_loadedPlugins.emplace(path, std::move(plugin));
		}
		catch (ElpidaException& e)
		{
			Logger::getInstance().log(Logger::LogType::Error, e.getMessage());
		}
	}

	void PluginLoader::unloadEverything()
	{
		_loadedPlugins.clear();
	}

} /* namespace Elpida */
