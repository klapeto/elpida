#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "Views/ConfigurationViews/ConfigurationView.hpp"

#include "ConfigurationViewPool.hpp"
#include "Models/BenchmarksModel.hpp"

namespace Elpida::Application
{
	BenchmarkConfigurationView::BenchmarkConfigurationView(const BenchmarksModel& benchmarksModel, ConfigurationViewPool& configurationViewPool)
		: QWidget(),
		  _ui(new Ui::BenchmarkConfigurationView), _benchmarksModel(benchmarksModel), _configurationViewPool(configurationViewPool), _currentBenchmark(nullptr)
	{
		_ui->setupUi(this);
		_ui->container->setLayout(new QVBoxLayout);

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

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		ClearViews();
		delete _ui;
	}

	void BenchmarkConfigurationView::ClearViews()
	{
		for (auto& pair: _rentedViews)
		{
			Remove(*pair.first);
		}
	}
	void BenchmarkConfigurationView::Add(const BenchmarkConfigurationModel& model)
	{
		auto layout = _ui->container->layout();
		auto view = _configurationViewPool.RentViewForModel(model);
		layout->addWidget(view);
		_rentedViews.insert({ &model, view });
	}

	void
	BenchmarkConfigurationView::Remove(const BenchmarkConfigurationModel& model)
	{
		auto view = _rentedViews.at(&model);
		auto layout = _ui->container->layout();
		layout->removeWidget(view);
		view->setParent(nullptr);
		_configurationViewPool.ReturnViewFromModel(model, view);

	}
}

