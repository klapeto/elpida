/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020 Ioannis Panagiotopoulos
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
#include <memory>
#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"

namespace Elpida
{
	class TaskConfigurationListItemViewBase;
	class TaskConfiguration;

	class ConfigurationViewsPool
	{
	public:
		std::unique_ptr<ConfigurationValueViewBase> rentViewForConfiguration(const std::shared_ptr<ConfigurationValueBase>& spec);
		void returnView(std::unique_ptr<ConfigurationValueViewBase>&& view);

		std::unique_ptr<TaskConfigurationListItemViewBase> rentViewForTaskList(TaskConfiguration& spec);
		void returnViewForTaskList(std::unique_ptr<TaskConfigurationListItemViewBase>&& view);

		ConfigurationViewsPool();
		virtual ~ConfigurationViewsPool() = default;
	private:
		std::stack<std::unique_ptr<ConfigurationValueViewBase>> _fileViews;
		std::stack<std::unique_ptr<ConfigurationValueViewBase>> _numberViews;
		std::stack<std::unique_ptr<TaskConfigurationListItemViewBase>> _taskListItemViews;

		template<typename TItem, typename TBase>
		std::unique_ptr<TBase> getFromStack(std::stack<std::unique_ptr<TBase>>& stack)
		{
			if (!stack.empty())
			{
				auto value = std::move(stack.top());
				stack.pop();
				return value;
			}
			else
			{
				return std::make_unique<TItem>();
			}
		}

		template<typename TItem, typename TBase>
		void fillPool(std::stack<std::unique_ptr<TBase>>& stack)
		{
			for (size_t i = 0; i < _initialPoolsSize; ++i)
			{
				stack.push(std::make_unique<TItem>());
			}
		}

		const size_t _initialPoolsSize = 5;
	};
}

#endif //APPS_QT_CONFIGURATIONVIEWSPOOL_HPP
