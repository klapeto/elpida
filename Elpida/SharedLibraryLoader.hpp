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

namespace Elpida
{

	class SharedLibraryLoader
	{
		public:
			const std::unordered_map<std::string, SharedLibrary>& getLoadedPlugins() const
			{
				return _loadedLibraries;
			}

			virtual void loadFromFolder(const std::string& path, const std::string& orderFile = std::string());
			void load(const std::string& path);
			void unload(const std::string& path);

			SharedLibraryLoader();
			virtual ~SharedLibraryLoader();

		protected:
			std::unordered_map<std::string, SharedLibrary> _loadedLibraries;
			void unloadEverything();
	};

} /* namespace Elpida */

#endif /* ELPIDA_SHAREDLIBRARYLOADER_HPP_ */
