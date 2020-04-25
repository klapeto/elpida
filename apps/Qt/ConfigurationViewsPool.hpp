//
// Created by klapeto on 22/4/20.
//

#ifndef APPS_QT_CONFIGURATIONVIEWSPOOL_HPP
#define APPS_QT_CONFIGURATIONVIEWSPOOL_HPP

#include <stack>
#include <Elpida/Engine/Configuration/ConfigurationType.hpp>

namespace Elpida
{
	class ConfigurationValueViewBase;
	class ConfigurationValueBase;
	class TaskConfigurationListItemViewBase;
	class TaskConfiguration;

	class ConfigurationViewsPool
	{
	public:
		ConfigurationValueViewBase* rentViewForConfiguration(const ConfigurationValueBase& spec);
		void returnView(ConfigurationValueViewBase* view);

		TaskConfigurationListItemViewBase* rentViewForTaskList(TaskConfiguration& spec);
		void returnViewForTaskList(TaskConfigurationListItemViewBase* view);

		ConfigurationViewsPool();
		virtual ~ConfigurationViewsPool();
	private:
		std::stack<ConfigurationValueViewBase*> _fileViews;
		std::stack<ConfigurationValueViewBase*> _numberViews;
		std::stack<TaskConfigurationListItemViewBase*> _taskListItemViews;

		template<typename TItem, typename TBase>
		TBase* getFromStack(std::stack<TBase*>& stack)
		{
			if (!stack.empty())
			{
				auto value = stack.top();
				stack.pop();
				return value;
			}
			else
			{
				return new TItem();
			}
		}

		template<typename TItem, typename TBase>
		void fillPool(std::stack<TBase*>& stack)
		{
			for (size_t i = 0; i < _initialPoolsSize; ++i)
			{
				stack.push(new TItem());
			}
		}

		template<typename TBase>
		static void destroyPool(std::stack<TBase*>& stack)
		{
			auto size = stack.size();
			for (auto i = 0u; i < size; ++i)
			{
				auto item = stack.top();
				delete item;
				stack.pop();
			}
		}

		const size_t _initialPoolsSize = 5;
	};
}


#endif //APPS_QT_CONFIGURATIONVIEWSPOOL_HPP
