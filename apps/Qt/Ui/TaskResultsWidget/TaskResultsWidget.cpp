#include "TaskResultsWidget.hpp"
#include "ui_TaskResultsWidget.h"

namespace Elpida
{

	TaskResultsWidget::TaskResultsWidget()
		:
		QWidget(),
		_ui(new Ui::TaskResultsWidget)
	{
		_ui->setupUi(this);
		_ui->tvResults->setModel(&_model);
	}

	TaskResultsWidget::~TaskResultsWidget()
	{
		delete _ui;
	}

} // namespace Elpida
