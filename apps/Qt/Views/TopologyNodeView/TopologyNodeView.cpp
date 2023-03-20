#include "TopologyNodeView.hpp"
#include "ui_TopologyNodeView.h"

namespace Elpida::Application
{
	TopologyNodeView::TopologyNodeView(QWidget* parent) :
			QFrame(parent),
			_ui(new Ui::TopologyNodeView)
	{
		_ui->setupUi(this);
	}

	TopologyNodeView::~TopologyNodeView()
	{
		delete _ui;
	}

}
