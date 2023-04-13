#include "StringConfigurationView.hpp"
#include "ui_StringConfigurationView.h"

namespace Elpida::Application
{

	StringConfigurationView::StringConfigurationView(QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::StringConfigurationView)
	{
		_ui->setupUi(this);
	}

	StringConfigurationView::~StringConfigurationView()
	{
		delete _ui;
	}

} // namespace Elpida
