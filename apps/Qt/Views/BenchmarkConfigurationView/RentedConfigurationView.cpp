/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2021 Ioannis Panagiotopoulos
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
// Created by klapeto on 9/5/21.
//

#include "RentedConfigurationView.hpp"

#include <Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp>
#include <QListWidget>
#include <QVBoxLayout>

#include "ConfigurationViewsPool.hpp"

namespace Elpida
{
	RentedConfigurationView::RentedConfigurationView(
			const BenchmarkConfiguration& benchmarkConfiguration,
			QListWidget& tasksList,
			QVBoxLayout& configurationContainer,
			ConfigurationViewsPool& configurationViewsPool)
			: _tasksList(tasksList),
			  _configurationContainer(configurationContainer),
			  _configurationViewsPool(configurationViewsPool)
	{
		auto& taskConfigurations = benchmarkConfiguration.getAllTaskConfigurations();
		_taskItems.reserve(taskConfigurations.size());

		for (auto& taskConfiguration : taskConfigurations)
		{
			auto taskItem = _configurationViewsPool.rentViewForTaskList(
					const_cast<TaskConfiguration&>(taskConfiguration));
			_tasksList.addItem(taskItem.get());
			_taskItems.push_back(std::move(taskItem));

			for (auto& confValuePair : taskConfiguration.getAllConfigurations())
			{
				auto& type = confValuePair.second->getConfigurationSpecification();
				if (!confValuePair.second->isReadOnly()
					&& type.getType() != ConfigurationType::Type::Custom
					&& type.getType() != ConfigurationType::Type::Function)
				{
					auto view = _configurationViewsPool.rentViewForConfiguration(confValuePair.second);
					view->show();
					view->setConfiguration(confValuePair.second);
					_configurationContainer.addWidget(view.get());
					_configurationViews.push_back(std::move(view));
				}
			}
		}
	}

	RentedConfigurationView::~RentedConfigurationView()
	{
		for (auto& item: _taskItems)
		{
			item->saveSetting();
			_tasksList.takeItem(_tasksList.row(item.get()));
			_configurationViewsPool.returnViewForTaskList(std::move(item));
		}

		for (auto& configView : _configurationViews)
		{
			configView->saveSetting();
			configView->hide();
			_configurationContainer.removeWidget(configView.get());
			configView->setParent(nullptr);
			_configurationViewsPool.returnView(std::move(configView));
		}
	}
}
