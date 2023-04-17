#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "Models/BenchmarkConfigurationInstanceModel.hpp"
#include "Views/ConfigurationViews/ConfigurationView.hpp"

#include "ConfigurationViewPool.hpp"

namespace Elpida::Application
{
	BenchmarkConfigurationView::BenchmarkConfigurationView(const BenchmarkConfigurationModel& configurationModel, ConfigurationViewPool& configurationViewPool)
		: QWidget(),
		  _ui(new Ui::BenchmarkConfigurationView), _configurationModel(configurationModel), _configurationViewPool(configurationViewPool)
	{
		_ui->setupUi(this);
		_ui->container->setLayout(new QVBoxLayout);

		_itemAddedSubscription = _configurationModel.ItemAdded().Subscribe([this](const CollectionItem<BenchmarkConfigurationInstanceModel>& item)
		{
			Add(item.GetValue());
		});

		_itemRemovedSubscription = _configurationModel.ItemRemoved().Subscribe([this](const CollectionItem<BenchmarkConfigurationInstanceModel>& item)
		{
			Remove(item.GetValue());
		});

		_clearedSubscription = _configurationModel.Cleared().Subscribe([this]()
		{
			ClearViews();
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
	void BenchmarkConfigurationView::Add(const BenchmarkConfigurationInstanceModel& model)
	{
		auto layout = _ui->container->layout();
		auto view = _configurationViewPool.RentViewForModel(model);
		layout->addWidget(view);
		_rentedViews.insert({ &model, view });
	}

	void
	BenchmarkConfigurationView::Remove(const BenchmarkConfigurationInstanceModel& model)
	{
		auto view = _rentedViews.at(&model);
		auto layout = _ui->container->layout();
		layout->removeWidget(view);
		view->setParent(nullptr);
		_configurationViewPool.ReturnViewFromModel(model, view);

	}
}

