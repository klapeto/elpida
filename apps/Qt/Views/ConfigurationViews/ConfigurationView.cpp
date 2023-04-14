//
// Created by klapeto on 14/4/2023.
//

#include "ConfigurationView.hpp"

namespace Elpida::Application
{
	ConfigurationView::ConfigurationView(QWidget* parent)
		: QWidget(parent), _model(nullptr)
	{

	}

	void ConfigurationView::SetModel(ConfigurationModel* model)
	{
		_model = model;
		OnModelSet();
	}
} // Application