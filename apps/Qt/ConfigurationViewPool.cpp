/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

//
// Created by klapeto on 17/4/2023.
//

#include "ConfigurationViewPool.hpp"

#include "Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.hpp"
#include "Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.hpp"
#include "Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.hpp"
#include "Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.hpp"
#include "Views/ConfigurationViews/BooleanConfigurationView/BooleanConfigurationView.hpp"

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
		case ConfigurationType::Boolean:
			view = GetOrCreate<BooleanConfigurationView>(_booleanViews);
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
			case ConfigurationType::Boolean:
				_booleanViews.push_back(std::move(ptr));
				break;
			}
			_rentedInstances.erase(itr);
		}
	}
} // Elpida::Application