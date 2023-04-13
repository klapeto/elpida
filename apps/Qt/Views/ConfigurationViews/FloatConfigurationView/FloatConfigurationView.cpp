#include "FloatConfigurationView.hpp"
#include "ui_FloatConfigurationView.h"

namespace Elpida::Application
{

	FloatConfigurationView::FloatConfigurationView(QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::FloatConfigurationView)
	{
		_ui->setupUi(this);
	}

	FloatConfigurationView::~FloatConfigurationView()
	{
		delete _ui;
	}

} // namespace Elpida
