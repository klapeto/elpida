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

#include "Elpida/SharedLibrary.hpp"
#include "Elpida/Types/Map.hpp"
#include "Elpida/Types/String.hpp"

namespace Elpida
{

	class SharedLibraryLoader
	{
		public:
			const Map<String, SharedLibrary>& getLoadedPlugins() const
			{
				return _loadedLibraries;
			}

			virtual void loadFromFolder(const String& path, const String& orderFile = String());
			void load(const String& path);
			void unload(const String& path);

			SharedLibraryLoader();
			virtual ~SharedLibraryLoader();

		protected:
			Map<String, SharedLibrary> _loadedLibraries;
			void unloadEverything();
	};

} /* namespace Elpida */

#endif /* ELPIDA_SHAREDLIBRARYLOADER_HPP_ */
