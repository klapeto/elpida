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

#include "Elpida/SharedLibraryLoader.hpp"

#include "Elpida/ElpidaException.hpp"

namespace Elpida
{

	const std::string SharedLibraryLoader::LibrariesExtension{ELPIDA_SHARED_LIBRARY_EXTENSION};

	void SharedLibraryLoader::load(const std::string& path)
	{
		_loadedLibraries.emplace(path, SharedLibrary(path));
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
