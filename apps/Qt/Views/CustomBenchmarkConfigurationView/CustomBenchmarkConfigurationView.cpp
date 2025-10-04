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

#include "CustomBenchmarkConfigurationView.hpp"
#include "ui_CustomBenchmarkConfigurationView.h"

#include <QSpacerItem>

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Views/ConfigurationViews/ConfigurationView.hpp"

#include "ConfigurationViewPool.hpp"
#include "Models/Custom/CustomBenchmarkModel.hpp"

namespace Elpida::Application
{
	CustomBenchmarkConfigurationView::CustomBenchmarkConfigurationView(const CustomBenchmarkModel& benchmarksModel, ConfigurationViewPool& configurationViewPool)
		: QWidget(),
		  _ui(new Ui::CustomBenchmarkConfigurationView), _benchmarksModel(benchmarksModel), _configurationViewPool(configurationViewPool), _currentBenchmark(nullptr)
	{
		_ui->setupUi(this);
		_ui->container->setLayout(new QVBoxLayout);
		_ui->container->layout()->addItem(new QSpacerItem(1,1, QSizePolicy::Expanding, QSizePolicy::Expanding));

		_currentBenchmarkChanged = _benchmarksModel.DataChanged().Subscribe([this]()
		{
			auto currentBenchmark = _benchmarksModel.GetSelectedBenchmark();
			if (_currentBenchmark == currentBenchmark) return;
			_currentBenchmark = currentBenchmark;
			ClearViews();
			if (currentBenchmark != nullptr)
			{
				for (auto& config: currentBenchmark->GetConfigurations())
				{
					Add(config);
				}
			}
		});
	}

	CustomBenchmarkConfigurationView::~CustomBenchmarkConfigurationView()
	{
		ClearViews();
		delete _ui;
	}

	void CustomBenchmarkConfigurationView::ClearViews()
	{
		for (auto& pair: _rentedViews)
		{
			Remove(*pair.first);
		}
	}
	void CustomBenchmarkConfigurationView::Add(const BenchmarkConfigurationModel& model)
	{
		auto layout = _ui->container->layout();
		auto view = _configurationViewPool.RentViewForModel(model);
		static_cast<QVBoxLayout*>(layout)->insertWidget(layout->count() - 1,view);
		_rentedViews.insert({ &model, view });
	}

	void
	CustomBenchmarkConfigurationView::Remove(const BenchmarkConfigurationModel& model)
	{
		auto view = _rentedViews.at(&model);
		auto layout = _ui->container->layout();
		layout->removeWidget(view);
		view->setParent(nullptr);
		_configurationViewPool.ReturnViewFromModel(model, view);

	}
}

