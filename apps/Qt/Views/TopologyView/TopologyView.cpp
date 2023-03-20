#include "TopologyView.hpp"
#include "ui_TopologyView.h"

namespace Elpida::Application
{
	TopologyView::TopologyView(TopologyNodeModel& model, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::TopologyView), _model(model)
	{
		_ui->setupUi(this);
	}

	TopologyView::~TopologyView()
	{
		delete _ui;
	}
}
