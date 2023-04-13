#include "IntegerConfigurationView.hpp"
#include "ui_IntegerConfigurationView.h"

namespace Elpida::Application
{

	IntegerConfigurationView::IntegerConfigurationView(QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::IntegerConfigurationView)
	{
		_ui->setupUi(this);
	}

	IntegerConfigurationView::~IntegerConfigurationView()
	{
		delete _ui;
	}

} // namespace Elpida
