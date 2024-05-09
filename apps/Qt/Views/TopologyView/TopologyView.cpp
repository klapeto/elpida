#include "TopologyView.hpp"
#include "ui_TopologyView.h"

#include "Layouts/FlowLayout.hpp"

#include "Views/TopologyNodeView/TopologyNodeView.hpp"
#include "Models/SystemInfo/TopologyModel.hpp"

namespace Elpida::Application
{
	TopologyView::TopologyView(TopologyModel& model, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::TopologyView), _model(model)
	{
		_ui->setupUi(this);
		_ui->saContainer->setAlignment(Qt::AlignCenter);
		_ui->saContainer->setWidgetResizable(false);
		std::optional<unsigned int> fastestProcessor =  _model.GetFastestProcessor();
		_ui->saContainer->setWidget(new TopologyNodeView(_model.GetRoot(), fastestProcessor));
	}

	TopologyView::~TopologyView()
	{
		delete _ui;
	}
}
