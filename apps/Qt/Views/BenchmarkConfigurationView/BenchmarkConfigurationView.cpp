#include "BenchmarkConfigurationView.hpp"
#include "ui_BenchmarkConfigurationView.h"

#include "Models/BenchmarksModel.hpp"

#include "Views/ConfigurationViews/FileConfigurationView/FileConfigurationView.hpp"
#include "Views/ConfigurationViews/FloatConfigurationView/FloatConfigurationView.hpp"
#include "Views/ConfigurationViews/IntegerConfigurationView/IntegerConfigurationView.hpp"
#include "Views/ConfigurationViews/StringConfigurationView/StringConfigurationView.hpp"

namespace Elpida::Application
{
	BenchmarkConfigurationView::BenchmarkConfigurationView()
		: QWidget(),
		  _ui(new Ui::BenchmarkConfigurationView), _benchmarkModel(nullptr)
	{
		_ui->setupUi(this);
		_ui->container->setLayout(new QVBoxLayout);
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

	void BenchmarkConfigurationView::SetModel(BenchmarkModel* benchmarkModel)
	{
		_benchmarkModel = benchmarkModel;
		ClearViews();

		if (_benchmarkModel != nullptr)
		{
			auto layout = _ui->container->layout();
			for (auto& config: _benchmarkModel->GetConfigurations())
			{
				auto view = RentView(const_cast<ConfigurationModel&>(config));
				view->SetModel(const_cast<ConfigurationModel*>(&config));
				layout->addWidget(view);
			}
		}
	}

	void BenchmarkConfigurationView::ClearViews()
	{
		auto layout = _ui->container->layout();

		for (auto& tuple: _createdWidgets)
		{
			auto view = std::get<ConfigurationView*>(tuple);
			layout->removeWidget(view);
			view->setParent(nullptr);
			ReturnView(std::get<ConfigurationModel*>(tuple), std::get<ConfigurationView*>(tuple));
		}

		_createdWidgets.clear();
	}

	ConfigurationView* BenchmarkConfigurationView::RentView(ConfigurationModel& configurationModel)
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
		_createdWidgets.emplace_back(&configurationModel, view);
		return view;
	}
	void
	BenchmarkConfigurationView::ReturnView(ConfigurationModel* configurationModel, ConfigurationView* configurationView)
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

