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
 * SharedLibraryLoader.cpp
 *
 *  Created on: 2 Ιουλ 2018
 *      Author: klapeto
 */

#include <iostream>

#include "Config.hpp"
#include "Elpida/TaskBatch.hpp"
#include "Elpida/Exceptions/ElpidaException.hpp"
#include "Elpida/Utilities/FileSystem.hpp"
#include "Elpida/Utilities/Logger.hpp"
#include <fstream>
#include <algorithm>
#include "Elpida/SharedLibraryLoader.hpp"

#if _elpida_linux
constexpr const char* LibraryExtension = ".so";
#elif _elpida_windows
constexpr const char* LibraryExtension = ".dll";
#endif

namespace Elpida
{

	SharedLibraryLoader::SharedLibraryLoader()
	{

	}

	void SharedLibraryLoader::loadFromFolder(const String& path, const String& orderFile)
	{
		unloadEverything();

		Array<String> loadFilenames;

		FileSystem::iterateDirectory(path, [&loadFilenames](const String& filePath)
		{
			if (filePath.find(LibraryExtension) != std::string::npos)
			{
				loadFilenames.push_back(filePath);
			}
		});

		if (FileSystem::fileExists(orderFile))
		{
			std::ifstream orderFileStream(orderFile, std::ios::in);
			if (orderFileStream.good())
			{
				String line;
				auto pred = [&line](const String& val)
				{	return val.find(line + LibraryExtension) != String::npos;};
				while (std::getline(orderFileStream, line))
				{
					auto itr = std::find_if(loadFilenames.begin(), loadFilenames.end(), pred);
					if (itr != loadFilenames.end())
					{
						if (FileSystem::fileExists(*itr))
						{
							load(*itr);
						}
						else
						{
							Logger::getInstance().log(Logger::LogType::Warning, "Failed to open referenced shared library '", line, '\'');
						}
					}
					else
					{
						Logger::getInstance().log(Logger::LogType::Warning,
						                          "A shared library referenced on '" + orderFile + "' file was not found: ", line);
					}
				}
				orderFileStream.close();
			}
		}
		else
		{
			Logger::getInstance().log(Logger::LogType::Warning, "'", orderFile, "' file",
			                          "was not found. Elpida will load the libraries in folder in unspecified order");
			for (const auto& file : loadFilenames)
			{
				load(file);
			}
		}
	}

	SharedLibraryLoader::~SharedLibraryLoader()
	{

	}

	void SharedLibraryLoader::load(const String& path)
	{
		try
		{
			_loadedLibraries.emplace(path, SharedLibrary(path));
		}
		catch (ElpidaException& e)
		{
			Logger::getInstance().log(Logger::LogType::Error, e.getMessage());
		}
	}

	void SharedLibraryLoader::unload(const String& path)
	{
		_loadedLibraries.erase(path);
	}

	void SharedLibraryLoader::unloadEverything()
	{
		_loadedLibraries.clear();
	}

} /* namespace Elpida */
