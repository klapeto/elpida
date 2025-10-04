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

#include "OsInfoView.hpp"
#include "ui_OsInfoView.h"

#include "Models/SystemInfo/OsInfoModel.hpp"

namespace Elpida::Application
{
	OsInfoView::OsInfoView(const OsInfoModel& osInfo, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::OsInfoView)
	{
		_ui->setupUi(this);

		_ui->lblCategoryValue->setText(QString::fromStdString(osInfo.GetCategory()));
		_ui->lblNameValue->setText(QString::fromStdString(osInfo.GetName()));
		_ui->lblVersionValue->setText(QString::fromStdString(osInfo.GetVersion()));
	}

	OsInfoView::~OsInfoView()
	{
		delete _ui;
	}
}

