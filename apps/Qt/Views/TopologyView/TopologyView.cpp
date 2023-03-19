#include "TopologyView.hpp"
#include "ui_TopologyView.h"

TopologyView::TopologyView(QWidget *parent) :
	QWidget(parent),
	_ui(new Ui::TopologyView)
{
	_ui->setupUi(this);
}

TopologyView::~TopologyView()
{
	delete _ui;
}
