//
// Created by klapeto on 17/4/2023.
//

#include "ConfigurationViewPool.hpp"

#include "Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.hpp"
#include "Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.hpp"
#include "Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.hpp"
#include "Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.hpp"

#include "Core/SettingsService.hpp"

namespace Elpida::Application
{
	ConfigurationViewPool::ConfigurationViewPool(SettingsService& settingsService)
		: _settingsService(settingsService)
	{

	}

	ConfigurationView*
	ConfigurationViewPool::RentViewForModel(const BenchmarkConfigurationModel& configurationModel)
	{
		std::unique_ptr<ConfigurationView> view;
		switch (configurationModel.GetType())
		{
		case ConfigurationType::Integer:
			view = GetOrCreate<IntegerConfigurationView>(_integerViews);
			break;
		case ConfigurationType::Float:
			view = GetOrCreate<FloatConfigurationView>(_floatViews);
			break;
		case ConfigurationType::String:
			view = GetOrCreate<StringConfigurationView>(_stringViews);
			break;
		case ConfigurationType::File:
			view = GetOrCreate<FileConfigurationView>(_fileViews);
			break;
		}

		BenchmarkConfigurationInstanceController
			controller(const_cast<BenchmarkConfigurationModel&>(configurationModel), _settingsService);
		CreatedInstance instance{ view.release(), std::move(controller) };
		auto& actualInstance = _rentedInstances.insert({ &configurationModel, std::move(instance) }).first->second;

		actualInstance.view->SetModel(&configurationModel, actualInstance.controller);

		return actualInstance.view;
	}

	void
	ConfigurationViewPool::ReturnViewFromModel(const BenchmarkConfigurationModel& configurationModel,
		ConfigurationView* view)
	{
		auto itr = _rentedInstances.find(&configurationModel);

		if (itr != _rentedInstances.end())
		{
			auto ptr = std::unique_ptr<ConfigurationView>(itr->second.view);
			switch (configurationModel.GetType())
			{
			case ConfigurationType::Integer:
				_integerViews.push_back(std::move(ptr));
				break;
			case ConfigurationType::Float:
				_floatViews.push_back(std::move(ptr));
				break;
			case ConfigurationType::String:
				_stringViews.push_back(std::move(ptr));
				break;
			case ConfigurationType::File:
				_fileViews.push_back(std::move(ptr));
				break;
			}
			_rentedInstances.erase(itr);
		}
	}
} // Elpida::Application