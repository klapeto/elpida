//
// Created by klapeto on 12/4/20.
//

#ifndef INCLUDE_ELPIDA_UTILITIES_PLUGIN_TASKBATCHESCONTAINERPLUGIN_HPP
#define INCLUDE_ELPIDA_UTILITIES_PLUGIN_TASKBATCHESCONTAINERPLUGIN_HPP

#include <vector>
#include "Elpida/Utilities/Plugin/Plugin.hpp"

namespace Elpida
{

	template<typename T>
	class TaskBatchesContainerPlugin : public Plugin<std::vector<T*>>
	{
	public:

		using Factory = TaskBatchesContainerPlugin<T>* (*)();

		const std::vector<T*>& getUnderlyingData() const override
		{
			return _data;
		}

		void add(T* data)
		{
			_data.push_back(data);
		}

		TaskBatchesContainerPlugin() = default;
		virtual ~TaskBatchesContainerPlugin()
		{
			for (auto ptr : _data)
			{
				delete ptr;
			}
		}
	private:
		std::vector<T*> _data;
	};
}


#endif //INCLUDE_ELPIDA_UTILITIES_PLUGIN_TASKBATCHESCONTAINERPLUGIN_HPP
