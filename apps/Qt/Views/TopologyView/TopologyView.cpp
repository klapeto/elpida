/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

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
