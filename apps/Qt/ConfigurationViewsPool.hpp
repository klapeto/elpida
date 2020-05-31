/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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
