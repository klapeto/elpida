#include "TaskBatchProperties.hpp"
#include "ui_TaskBatchProperties.h"

TaskBatchProperties::TaskBatchProperties(QWidget *parent) :
	QDialog(parent),
	ui(new Ui::TaskBatchProperties)
{
	ui->setupUi(this);

}

TaskBatchProperties::~TaskBatchProperties()
{
	delete ui;
}
