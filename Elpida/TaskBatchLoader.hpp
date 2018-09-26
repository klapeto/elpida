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

#ifndef ELPIDA_TASKBATCHLOADER_HPP_
#define ELPIDA_TASKBATCHLOADER_HPP_

#include "Elpida/Types/Map.hpp"
#include "Elpida/Types/Array.hpp"
#include "Elpida/Types/String.hpp"
#include "Elpida/Plugin.hpp"

namespace Elpida
{
	class TaskBatch;

	class TaskBatchLoader
	{
		public:
			TaskBatch& getBatch(const String& name);
			const Map<String, TaskBatch*>& getBatches() const
			{
				return _loadedObjects;
			}

			void loadFromFolder(const String& path);

			TaskBatchLoader();
			~TaskBatchLoader();
		private:
			Map<String, TaskBatch*> _loadedObjects;
			Array<Plugin> _loadedPlugins;
			void loadLibraryAndGetTaskBatch(const String& path);
			void* getFunctionAddress(const String& name);
	};

} /* namespace Elpida */

#endif /* ELPIDA_TASKBATCHLOADER_HPP_ */
