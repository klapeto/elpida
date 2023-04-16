#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "Models/BenchmarkConfigurationInstanceModel.hpp"

#include "Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.hpp"
#include "Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.hpp"
#include "Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.hpp"
#include "Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.hpp"

namespace Elpida::Application
{
	BenchmarkConfigurationView::BenchmarkConfigurationView(BenchmarkConfigurationModel& configurationModel)
		: QWidget(),
		  _ui(new Ui::BenchmarkConfigurationView), _configurationModel(configurationModel)
	{
		_ui->setupUi(this);
		_ui->container->setLayout(new QVBoxLayout);

		_itemAddedSubscription = _configurationModel.ItemAdded().Subscribe([this](CollectionItem<BenchmarkConfigurationInstanceModel>& item)
		{
			auto layout = _ui->container->layout();
			auto view = RentView(const_cast<BenchmarkConfigurationInstanceModel&>(item.GetValue()));
			view->SetModel(const_cast<BenchmarkConfigurationInstanceModel*>(&item.GetValue()));
			layout->addWidget(view);
		});

		_itemRemovedSubscription = _configurationModel.ItemRemoved().Subscribe([this](CollectionItem<BenchmarkConfigurationInstanceModel>& item)
		{
			auto model = &item.GetValue();
			auto view = _createdWidgets.at(model);
			Remove(&item.GetValue(), view);
			_createdWidgets.erase(model);
		});

		_clearedSubscription = _configurationModel.Cleared().Subscribe([this]()
		{
			ClearViews();
		});
	}

	BenchmarkConfigurationView::~BenchmarkConfigurationView()
	{
		ClearViews();
		for (auto view: _fileViews)
		{
			delete view;
		}

		for (auto view: _floatViews)
		{
			delete view;
		}

		for (auto view: _integerViews)
		{
			delete view;
		}

		for (auto view: _stringViews)
		{
			delete view;
		}
		delete _ui;
	}

	void BenchmarkConfigurationView::ClearViews()
	{
		for (auto& pair: _createdWidgets)
		{
			Remove(pair.first, pair.second);
		}

		_createdWidgets.clear();
	}

	void
	BenchmarkConfigurationView::Remove(BenchmarkConfigurationInstanceModel* model, ConfigurationView* view)
	{
		auto layout = _ui->container->layout();
		layout->removeWidget(view);
		view->setParent(nullptr);
		ReturnView(model, view);
	}

	ConfigurationView* BenchmarkConfigurationView::RentView(BenchmarkConfigurationInstanceModel& configurationModel)
	{
		ConfigurationView* view = nullptr;
		switch (configurationModel.GetType())
		{
		case ConfigurationType::Integer:
			view = GetOrCreate(_integerViews, []()
			{ return new IntegerConfigurationView(); });
			break;
		case ConfigurationType::Float:
			view = GetOrCreate(_floatViews, []()
			{ return new FloatConfigurationView(); });
			break;
		case ConfigurationType::String:
			view = GetOrCreate(_stringViews, []()
			{ return new StringConfigurationView(); });
			break;
		case ConfigurationType::File:
			view = GetOrCreate(_fileViews, []()
			{ return new FileConfigurationView(); });
			break;
		}
		_createdWidgets.insert({ &configurationModel, view });
		return view;
	}

	void
	BenchmarkConfigurationView::ReturnView(BenchmarkConfigurationInstanceModel* configurationModel, ConfigurationView* configurationView)
	{
		switch (configurationModel->GetType())
		{
		case ConfigurationType::Integer:
			_integerViews.push_back(configurationView);
			break;
		case ConfigurationType::Float:
			_floatViews.push_back(configurationView);
			break;
		case ConfigurationType::String:
			_stringViews.push_back(configurationView);
			break;
		case ConfigurationType::File:
			_fileViews.push_back(configurationView);
			break;
		}
	}
}

