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

#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "ConfigurationViewsPool.hpp"
#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"

#include <Elpida/Engine/Configuration/Concrete/BenchmarkConfiguration.hpp>
#include <Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>

namespace Elpida
{

	BenchmarkConfigurationView::BenchmarkConfigurationView(BenchmarkConfigurationModel& benchmarkConfigurationModel,
		ConfigurationViewsPool& configurationViewsPool)
		: QWidget(nullptr), _benchmarkConfigurationModel(benchmarkConfigurationModel),
		  _configurationViewsPool(configurationViewsPool),
		  _ui(new Ui::BenchmarkConfigurationView)
	{
		_ui->setupUi(this);

		_containerLayout = new QVBoxLayout;
		_ui->scrollAreaWidgetContents->setLayout(_containerLayout);
		_dataChangedSubscription = &_benchmarkConfigurationModel.dataChanged.subscribe([this]()
		{
			emit dataChanged();
		});
		QWidget::connect(this,
			&BenchmarkConfigurationView::dataChanged,
			this,
			&BenchmarkConfigurationView::dataChangedHandler);
		QWidget::connect(_ui->lwTasks,
			&QListWidget::itemClicked,
			this,
			&BenchmarkConfigurationView::taskListItemClicked);
	}

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		_dataChangedSubscription->unsubscribe();
		returnAllTaskListItems();
		returnAllViewsToPool();
		delete _ui;
	}

	void BenchmarkConfigurationView::dataChangedHandler()
	{
		returnAllViewsToPool();
		returnAllTaskListItems();
		auto configuration = _benchmarkConfigurationModel.getBenchmarkConfiguration();
		for (auto& taskConfPair: configuration->getAllTaskConfigurations())
		{
			auto itm = _configurationViewsPool.rentViewForTaskList(const_cast<TaskConfiguration&>(taskConfPair));
			_ui->lwTasks->addItem(itm);
			for (auto& confValuePair: taskConfPair.getAllConfigurations())
			{
				if (!confValuePair.second->isReadOnly())
				{
					auto view = _configurationViewsPool.rentViewForConfiguration(*confValuePair.second);
					view->show();
					_rentedViews.push_back(view);
					view->setConfiguration(confValuePair.second);
					_containerLayout->addWidget(view);
				}
			}
		}
	}

	void BenchmarkConfigurationView::returnAllTaskListItems() const
	{
		auto size = _ui->lwTasks->count();
		for (auto i = size - 1; i >= 0; --i)
		{
			auto itm = (TaskConfigurationListItemViewBase*)_ui->lwTasks->takeItem(i);
			if (itm != nullptr)
			{
				itm->saveSetting();
				_configurationViewsPool.returnViewForTaskList(itm);
			}
		}
	}

	void BenchmarkConfigurationView::returnAllViewsToPool()
	{
		for (auto view : _rentedViews)
		{
			view->saveSetting();
			_containerLayout->removeWidget(view);
			view->setParent(nullptr);
			_configurationViewsPool.returnView(view);
			view->hide();
		}
		_rentedViews.clear();
	}

	void BenchmarkConfigurationView::taskListItemClicked(QListWidgetItem* item)
	{
		((TaskConfigurationListItemViewBase*)item)->saveSetting();
	}

} // namespace Elpida
