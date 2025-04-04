//
// Created by klapeto on 14/4/2023.
//

#include "ConfigurationView.hpp"

namespace Elpida::Application
{
	ConfigurationView::ConfigurationView(QWidget* parent)
		: QWidget(parent), _controller(nullptr), _model(nullptr), _settingModel(false)
	{

	}

	void
	ConfigurationView::SetModel(const BenchmarkConfigurationModel* model, BenchmarkConfigurationInstanceController& controller)
	{
		_model = model;
		_controller = &controller;
		_settingModel = true;
		OnModelSet();
		_settingModel = false;
	}
} // Application