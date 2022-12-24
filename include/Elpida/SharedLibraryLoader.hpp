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
 * SharedLibraryLoader.hpp
 *
 *  Created on: 2 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_SHAREDLIBRARYLOADER_HPP_
#define ELPIDA_SHAREDLIBRARYLOADER_HPP_

#include <string>
#include <unordered_map>

#include "Elpida/SharedLibrary.hpp"
#include "Elpida/Config.hpp"

namespace Elpida
{

	class SharedLibraryLoader final
	{
	public:
		static inline constexpr const char* LibrariesExtension{ELPIDA_SHARED_LIBRARY_EXTENSION};

		const std::unordered_map<std::string, SharedLibrary>& getLoadedLibraries() const
		{
			return _loadedLibraries;
		}

		const SharedLibrary& load(const std::string& path);
		void unload(const std::string& path);

		void unloadAll();

		SharedLibraryLoader() = default;
		~SharedLibraryLoader() = default;
	protected:
		std::unordered_map<std::string, SharedLibrary> _loadedLibraries;
	};

} /* namespace Elpida */

#endif /* ELPIDA_SHAREDLIBRARYLOADER_HPP_ */
