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
 * TaskBatchLoader.hpp
 *
 *  Created on: 10 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TOOLS_CLI_TASKBATCHLOADER_HPP_
#define TOOLS_CLI_TASKBATCHLOADER_HPP_

#include "Elpida/SharedLibraryLoader.hpp"

namespace Elpida
{
	class TaskBatch;

	namespace CLI
	{

		class TaskBatchLoader final: public SharedLibraryLoader
		{
			public:

				TaskBatch& getBatch(const String& name);

				const Map<String, TaskBatch*>& getBatches() const
				{
					return _loadedObjects;
				}

				void loadFromFolder(const String& path, const String& orderFile = String()) override;

				TaskBatchLoader()
						: SharedLibraryLoader()
				{

				}
				~TaskBatchLoader()
				{

				}
			private:
				Map<String, TaskBatch*> _loadedObjects;
				void* getFunctionAddress(const String& name);
		};

	} /* namespace CLI */
} /* namespace Elpida */

#endif /* TOOLS_CLI_TASKBATCHLOADER_HPP_ */
