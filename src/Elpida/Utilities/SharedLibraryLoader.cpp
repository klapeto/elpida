/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#include "Elpida/Utilities/SharedLibraryLoader.hpp"

namespace Elpida
{

	const SharedLibrary& SharedLibraryLoader::load(const std::string& path)
	{
		return _loadedLibraries.emplace(path, SharedLibrary(path)).first->second;
	}

	void SharedLibraryLoader::unload(const std::string& path)
	{
		_loadedLibraries.erase(path);
	}

	void SharedLibraryLoader::unloadAll()
	{
		_loadedLibraries.clear();
	}

} /* namespace Elpida */
