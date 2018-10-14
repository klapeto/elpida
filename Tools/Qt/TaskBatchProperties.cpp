/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#include "TaskBatchProperties.hpp"
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>
#include <QMessageBox>
#include "ui_TaskBatchProperties.h"

namespace Elpida
{

	TaskBatchProperties::TaskBatchProperties(QWidget *parent)
			: QDialog(parent), _ui(new Ui::TaskBatchProperties), _page(nullptr)
	{
		_ui->setupUi(this);

	}

	TaskBatchProperties::~TaskBatchProperties()
	{
		delete _ui;
	}

	void TaskBatchProperties::setPage(QtTaskBatchWrapper* widget)
	{
		auto layout = (QVBoxLayout*) _ui->wTaskBatchPropertiesContainer->layout();
		if (layout == nullptr)
		{
			layout = new QVBoxLayout;
			_ui->wTaskBatchPropertiesContainer->setLayout(layout);
		}
		auto item = layout->itemAt(0);
		layout->removeItem(item);
		delete item;
		layout->addWidget(widget, 0, 0);
		_page = widget;
	}

	void TaskBatchProperties::accept()
	{
		if (_page != nullptr)
		{
			try
			{
				_page->validateConfiguration();
				QDialog::accept();
			}
			catch (ElpidaException& e)
			{
				QMessageBox::critical(_ui->wTaskBatchPropertiesContainer, "Error", QString::fromStdString(e.getMessage()), QMessageBox::StandardButton::Ok);
			}
		}
	}

}  // namespace Elpida

