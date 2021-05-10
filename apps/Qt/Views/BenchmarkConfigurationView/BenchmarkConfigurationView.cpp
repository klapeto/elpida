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

#include "Models/BenchmarkConfigurationsCollectionModel.hpp"
#include "Models/BenchmarkRunnerModel.hpp"
#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"
#include "ConfigurationViewsPool.hpp"

#include <Elpida/Engine/Configuration/Concrete/TaskConfiguration.hpp>

namespace Elpida
{

	BenchmarkConfigurationView::BenchmarkConfigurationView(
			BenchmarkConfigurationsCollectionModel& benchmarkConfigurationsCollectionModel,
			ConfigurationViewsPool& configurationViewsPool,
			const SelectedBenchmarksModel& selectionModel,
			const BenchmarkRunnerModel& runnerModel)
			: QWidget(nullptr),
			  CollectionModelObserver<SelectedBenchmarksModel::Pair>(selectionModel),
			  _benchmarkConfigurationsCollectionModel(benchmarkConfigurationsCollectionModel),
			  _configurationViewsPool(configurationViewsPool),
			  _ui(new Ui::BenchmarkConfigurationView),
			  _runnerModel(runnerModel)
	{
		_ui->setupUi(this);

		_containerLayout = new QVBoxLayout;
		_ui->scrollAreaWidgetContents->setLayout(_containerLayout);

		QWidget::connect(_ui->lwTasks,
				&QListWidget::itemClicked,
				this,
				&BenchmarkConfigurationView::taskListItemClicked);

		QWidget::connect(this, &BenchmarkConfigurationView::onRunningChanged, this,
				&BenchmarkConfigurationView::updateRunningChanged);

		_runnerSubscription = &_runnerModel.runningChanged.subscribe([this](auto r)
		{ emit onRunningChanged(); });
	}

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		_runnerSubscription->unsubscribe();
		delete _ui;
	}

	void BenchmarkConfigurationView::taskListItemClicked(QListWidgetItem* item)
	{
		((TaskConfigurationListItemViewBase*)item)->saveSetting();
	}

	void BenchmarkConfigurationView::onCollectionCleared()
	{
		_rentedViews.clear();
	}

	void BenchmarkConfigurationView::onItemAdded(const SelectedBenchmarksModel::Pair& item)
	{
		auto itr = _rentedViews.find(item.second.get().getUuid());
		if (itr == _rentedViews.end())
		{
			auto itm = RentedConfigurationView(
					_benchmarkConfigurationsCollectionModel.get(item.second.get().getUuid()),
					*_ui->lwTasks,
					*_containerLayout,
					_configurationViewsPool);
			_rentedViews.emplace(item.second.get().getUuid(), std::move(itm));
		}
	}

	void BenchmarkConfigurationView::onItemRemoved(const SelectedBenchmarksModel::Pair& item)
	{
		_rentedViews.erase(item.second.get().getUuid());
	}

	void BenchmarkConfigurationView::updateRunningChanged()
	{
		setEnabled(!_runnerModel.isRunning());
	}
} // namespace Elpida
