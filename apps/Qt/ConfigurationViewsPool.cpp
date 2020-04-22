//
// Created by klapeto on 22/4/20.
//

#include "ConfigurationViewsPool.hpp"
#include <Elpida/Engine/Configuration/ConfigurationSpecificationBase.hpp>
#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>
#include "Views/ConfigurationViews/FileInputView/FileInputView.hpp"
#include <Elpida/ElpidaException.hpp>
#include <Elpida/Config.hpp>

namespace Elpida
{
	ConfigurationViewsPool::ConfigurationViewsPool()
	{
		fillPool<FileInputView>(_fileViews);
	}

	ConfigurationViewsPool::~ConfigurationViewsPool()
	{
		destroyPool(_fileViews);
	}

	void ConfigurationViewsPool::returnView(ConfigurationViewBase* view)
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

	ConfigurationViewBase* ConfigurationViewsPool::rentViewForConfiguration(const ConfigurationValueBase& spec)
	{
		switch (spec.getConfigurationSpecification().getType())
		{
		case ConfigurationType::FilePath:
			return getFromStack<FileInputView>(_fileViews);
		default:
			throw ElpidaException(FUNCTION_NAME, "Not implemented!");
		}
	}

	void ConfigurationViewsPool::destroyPool(std::stack<ConfigurationViewBase*>& stack)
	{
		auto size = stack.size();
		for (auto i = 0u; i < size; ++i)
		{
			auto item = stack.top();
			delete item;
			stack.pop();
		}
	}
}