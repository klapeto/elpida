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

		template<typename TR, typename ... TArgs>
		void constructAndAddNew(TArgs&& ... args)
		{
			_batches.push_back(new TR(std::forward<TArgs>(args)...));
		}

		template<typename TR, typename ... TArgs>
		static TR* constructNew(TArgs&& ... args)
		{
			return new TR(std::forward<TArgs>(args)...);
		}

		const std::vector<T*>& getUnderlyingData() const override
		{
			return _batches;
		}

		static TaskBatchesContainerPlugin<T>* createNew()
		{
			return new TaskBatchesContainerPlugin<T>();
		}

		virtual ~TaskBatchesContainerPlugin()
		{
			for (auto ptr : _batches)
			{
				delete ptr;
			}
		}
	private:
	public:
		TaskBatchesContainerPlugin() = default;
	private:
		std::vector<T*> _batches;
	};
}


#endif //INCLUDE_ELPIDA_UTILITIES_PLUGIN_TASKBATCHESCONTAINERPLUGIN_HPP
