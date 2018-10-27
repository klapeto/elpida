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
 * PluginLoader.hpp
 *
 *  Created on: 2 Ιουλ 2018
 *      Author: klapeto
 */

#ifndef ELPIDA_PLUGINLOADER_HPP_
#define ELPIDA_PLUGINLOADER_HPP_

#include "Elpida/Types/Map.hpp"
#include "Elpida/Types/String.hpp"
#include "Elpida/Plugin.hpp"

namespace Elpida
{

	class PluginLoader
	{
		public:
			const Map<String, Plugin>& getLoadedPlugins() const
			{
				return _loadedPlugins;
			}

			virtual void loadFromFolder(const String& path);
			void loadPlugin(const String& path);

			PluginLoader();
			virtual ~PluginLoader();
		protected:
			Map<String, Plugin> _loadedPlugins;
			void unloadEverything();
	};

} /* namespace Elpida */

#endif /* ELPIDA_PLUGINLOADER_HPP_ */
