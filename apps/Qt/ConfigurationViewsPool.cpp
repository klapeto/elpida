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

#include "ConfigurationViewsPool.hpp"
#include <Elpida/Engine/Configuration/Specification/ConfigurationSpecificationBase.hpp>
#include <Elpida/Engine/Configuration/Concrete/ConfigurationValue.hpp>
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Config.hpp>

#include "Views/ConfigurationViews/FileInputView/FileInputView.hpp"
#include "Views/ConfigurationViews/TaskListItemView/TaskListItemView.hpp"
#include "Views/ConfigurationViews/NumberInputView/NumberInputView.hpp"

namespace Elpida
{
	ConfigurationViewsPool::ConfigurationViewsPool()
	{
		fillPool<FileInputView>(_fileViews);
		fillPool<TaskListItemView>(_taskListItemViews);
		fillPool<NumberInputView>(_numberViews);
	}

	void ConfigurationViewsPool::returnView(std::unique_ptr<ConfigurationValueViewBase>&& view)
	{
		if (view)
		{
			auto type = view->getConfiguration()->getConfigurationSpecification().getType();
			view->setConfiguration(nullptr);
			switch (type)
			{
			case ConfigurationType::Type::FilePath:
				_fileViews.push(std::move(view));
				break;
			case ConfigurationType::Type::Float:
			case ConfigurationType::Type::Int:
			case ConfigurationType::Type::UnsignedInt:
				_numberViews.push(std::move(view));
				break;
			default:
				throw ElpidaException(FUNCTION_NAME, "Not implemented!");
			}
		}
	}

	std::unique_ptr<ConfigurationValueViewBase> ConfigurationViewsPool::rentViewForConfiguration(const std::shared_ptr<ConfigurationValueBase>& spec)
	{
		switch (spec->getConfigurationSpecification().getType())
		{
		case ConfigurationType::Type::FilePath:
			return getFromStack<FileInputView>(_fileViews);
		case ConfigurationType::Type::Float:
		case ConfigurationType::Type::Int:
		case ConfigurationType::Type::UnsignedInt:
			return getFromStack<NumberInputView>(_numberViews);
		default:
			throw ElpidaException(FUNCTION_NAME, "Not implemented!");
		}
	}

	std::unique_ptr<TaskConfigurationListItemViewBase> ConfigurationViewsPool::rentViewForTaskList(TaskConfiguration& spec)
	{
		auto view = getFromStack<TaskListItemView>(_taskListItemViews);
		view->setTaskConfiguration(spec);
		return view;
	}

	void ConfigurationViewsPool::returnViewForTaskList(std::unique_ptr<TaskConfigurationListItemViewBase>&& view)
	{
		if (view)
		{
			view->setTaskConfiguration(std::nullopt);
			_taskListItemViews.push(std::move(view));
		}
	}
}