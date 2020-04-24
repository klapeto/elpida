//
// Created by klapeto on 22/4/20.
//

#include "ConfigurationViewsPool.hpp"
#include <Elpida/Engine/Configuration/ConfigurationSpecificationBase.hpp>
#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>
#include "Views/ConfigurationViews/FileInputView/FileInputView.hpp"
#include "Views/ConfigurationViews/TaskListItemView/TaskListItemView.hpp"
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Config.hpp>

namespace Elpida
{
	ConfigurationViewsPool::ConfigurationViewsPool()
	{
		fillPool<FileInputView>(_fileViews);
		fillPool<TaskListItemView>(_taskListItemViews);
	}

	ConfigurationViewsPool::~ConfigurationViewsPool()
	{
		destroyPool(_fileViews);
		destroyPool(_taskListItemViews);
	}

	void ConfigurationViewsPool::returnView(ConfigurationValueViewBase* view)
	{
		if (view != nullptr)
		{
			switch (view->getConfiguration()->getConfigurationSpecification().getType())
			{
			case ConfigurationType::FilePath:
				_fileViews.push(view);
				break;
			default:
				throw ElpidaException(FUNCTION_NAME, "Not implemented!");
			}
			view->setConfiguration(nullptr);
		}
	}

	ConfigurationValueViewBase* ConfigurationViewsPool::rentViewForConfiguration(const ConfigurationValueBase& spec)
	{
		switch (spec.getConfigurationSpecification().getType())
		{
		case ConfigurationType::FilePath:
			return getFromStack<FileInputView>(_fileViews);
		default:
			throw ElpidaException(FUNCTION_NAME, "Not implemented!");
		}
	}

	TaskConfigurationListItemViewBase* ConfigurationViewsPool::rentViewForTaskList(TaskConfiguration& spec)
	{
		auto view = getFromStack<TaskListItemView>(_taskListItemViews);
		view->setTaskConfiguration(&spec);
		return view;
	}

	void ConfigurationViewsPool::returnViewForTaskList(TaskConfigurationListItemViewBase* view)
	{
		if (view != nullptr)
		{
			_taskListItemViews.push(view);
			view->setTaskConfiguration(nullptr);
		}
	}
}