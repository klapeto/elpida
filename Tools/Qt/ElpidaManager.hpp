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

#ifndef ELPIDAMANAGER_HPP
#define ELPIDAMANAGER_HPP

#include <string>

#include <Elpida/PluginLoader.hpp>
#include <Elpida/Types/String.hpp>
#include <Elpida/Utilities/NonCopyable.hpp>
#include <Elpida/Types/Map.hpp>
#include <Elpida/Types/Array.hpp>

namespace Elpida
{
	class ElpidaManager final: NonCopyable
	{
		public:

			void setPluginDirectory(const String& directory)
			{
				_batchesDirectory = directory;
			}

			const PluginLoader& getPluginLoader() const
			{
				return _batchLoader;
			}

			void reloadPlugins();

			ElpidaManager();
			~ElpidaManager();
		private:
			PluginLoader _batchLoader;
			String _batchesDirectory;
	};
}

#endif // ELPIDAMANAGER_HPP
