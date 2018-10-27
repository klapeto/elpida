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
			auto pred = [&line](const String& val)
			{	return val.find(line + TASK_ENDING) != String::npos;};
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
						std::cerr << "Failed to open referenced plugin '" + line << std::endl;
					}
				}
				else
				{
					std::cerr << "A plugin referenced on 'plugins' file was not found: " + line << std::endl;
				}
			}
			orderFile.close();
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
			std::cerr << "Error: " + e.getMessage() << std::endl;
		}
	}

	void PluginLoader::unloadEverything()
	{
		_loadedPlugins.clear();
	}

} /* namespace Elpida */
