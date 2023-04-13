#include "FileConfigurationView.hpp"
#include "ui_FileConfigurationView.h"

namespace Elpida::Application
{

	FileConfigurationView::FileConfigurationView(QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::FileConfigurationView)
	{
		_ui->setupUi(this);
	}

	FileConfigurationView::~FileConfigurationView()
	{
		delete _ui;
	}

} // namespace Elpida
