#include "TaskBatchProperties.hpp"
#include "ui_TaskBatchProperties.h"

TaskBatchProperties::TaskBatchProperties(QWidget *parent)
		: QDialog(parent), ui(new Ui::TaskBatchProperties)
{
	ui->setupUi(this);

}

TaskBatchProperties::~TaskBatchProperties()
{
	delete ui;
}

void TaskBatchProperties::setPage(QWidget* widget)
{
	auto& children = ui->gbTaskBatchContainer->children();
	if (children.size() > 0)
	{
		children.first()->setParent(nullptr);
	}
	widget->setParent(ui->gbTaskBatchContainer);
}
