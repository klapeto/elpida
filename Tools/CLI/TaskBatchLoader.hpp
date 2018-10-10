/*
 * TaskBatchLoader.hpp
 *
 *  Created on: 10 Οκτ 2018
 *      Author: klapeto
 */

#ifndef TOOLS_CLI_TASKBATCHLOADER_HPP_
#define TOOLS_CLI_TASKBATCHLOADER_HPP_

#include <Elpida/PluginLoader.hpp>

namespace Elpida
{
	class TaskBatch;

	namespace CLI
	{

		class TaskBatchLoader final: public PluginLoader
		{
			public:

				TaskBatch& getBatch(const String& name);

				const Map<String, TaskBatch*>& getBatches() const
				{
					return _loadedObjects;
				}

				void loadFromFolder(const String& path);

				TaskBatchLoader()
						: PluginLoader()
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
