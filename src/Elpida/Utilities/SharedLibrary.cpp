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
 * SharedLibrary.cpp
 *
 *  Created on: 26 Σεπ 2018
 *      Author: klapeto
 */

#include "Elpida/Utilities/SharedLibrary.hpp"

#include "Elpida/Config.hpp"
#include "Elpida/Common/ElpidaException.hpp"
#include "Elpida/Utilities/ValueUtilities.hpp"

namespace Elpida
{

	SharedLibrary::SharedLibrary(SharedLibrary&& other) noexcept
	{
		this->_handle = other._handle;
		other._handle = nullptr;
	}

	SharedLibrary& SharedLibrary::operator=(SharedLibrary&& other) noexcept
	{
		this->_handle = other._handle;
		other._handle = nullptr;
		return *this;
	}

	SharedLibrary::SharedLibrary(const std::string& libraryPath)
			: _path(libraryPath)
	{
		_handle = loadLibrary(libraryPath);

		if (_handle == nullptr)
		{
			throw ElpidaException("Plugin", Vu::Cs("Error loading plugin: '",libraryPath, "' -> ", getLoadError()));
		}
	}

	SharedLibrary::~SharedLibrary()
	{
		if (_handle != nullptr)
		{
			unloadLibrary(_handle);
		}
	}
}
/* namespace Elpida */
