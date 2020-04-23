#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "ConfigurationViewsPool.hpp"
#include "Views/ConfigurationViews/ConfigurationViewBase.hpp"
#include <Elpida/Engine/Configuration/BenchmarkConfiguration.hpp>
#include <Elpida/Engine/Configuration/TaskConfigurationSpecifications.hpp>
#include <Elpida/Engine/Configuration/TaskConfiguration.hpp>
#include <Elpida/Engine/Benchmark.hpp>

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
	}

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		_dataChangedSubscription->unsubscribe();
		delete _ui;
	}

	void BenchmarkConfigurationView::dataChangedHandler()
	{
		returnAllViewsToPool();
		auto configuration = _benchmarkConfigurationModel.getBenchmarkConfiguration();

		for (auto& taskConfPair: configuration->getAllTaskConfigurations())
		{
			for (auto& taskConfValuePair: taskConfPair.second.getAllConfigurations())
			{
				auto view = _configurationViewsPool.rentViewForConfiguration(*taskConfValuePair.second);
				view->show();
				_rentedViews.push_back(view);
				view->setConfiguration(taskConfValuePair.second);
				_containerLayout->addWidget(view);
			}
		}
	}

	void BenchmarkConfigurationView::returnAllViewsToPool()
	{

		for (auto view : _rentedViews)
		{
			_configurationViewsPool.returnView(view);
			_containerLayout->removeWidget(view);
			view->hide();
		}
		_rentedViews.clear();
	}

} // namespace Elpida
