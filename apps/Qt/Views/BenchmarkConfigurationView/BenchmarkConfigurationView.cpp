#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "ConfigurationViewsPool.hpp"
#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"

#include <Elpida/Engine/Configuration/BenchmarkConfiguration.hpp>
#include <Elpida/Engine/Configuration/TaskConfigurationSpecifications.hpp>
#include <Elpida/Engine/Configuration/TaskConfiguration.hpp>
#include <Elpida/Engine/Task/TaskSpecification.hpp>
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
		QWidget::connect(_ui->lwTasks,
			&QListWidget::itemClicked,
			this,
			&BenchmarkConfigurationView::taskListItemClicked);
	}

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		_dataChangedSubscription->unsubscribe();
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
				auto view = _configurationViewsPool.rentViewForConfiguration(*confValuePair.second);
				view->show();
				_rentedViews.push_back(view);
				view->setConfiguration(confValuePair.second);
				_containerLayout->addWidget(view);
			}
		}
	}

	void BenchmarkConfigurationView::returnAllTaskListItems() const
	{
		auto size = _ui->lwTasks->count();
		for (auto i = size; i >= 0; --i)
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
			_configurationViewsPool.returnView(view);
			_containerLayout->removeWidget(view);
			view->hide();
		}
		_rentedViews.clear();
	}

	void BenchmarkConfigurationView::taskListItemClicked(QListWidgetItem* item)
	{
		((TaskConfigurationListItemViewBase*)item)->saveSetting();
	}

} // namespace Elpida
