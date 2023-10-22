#include "CustomBenchmarkConfigurationView.hpp"
#include "ui_CustomBenchmarkConfigurationView.h"

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
		layout->addWidget(view);
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

