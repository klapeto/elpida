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

#include "CpuInfoView.hpp"
#include "ui_CpuInfoView.h"

#include "Layouts/FlowLayout.hpp"

#include "Elpida/Core/ValueUtilities.hpp"
#include "Models/SystemInfo/CpuInfoModel.hpp"

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	CpuInfoView::CpuInfoView(const CpuInfoModel& cpuInfo, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::CpuInfoView)
	{
		_ui->setupUi(this);
		_ui->lblArchitectureValue->setText(QString::fromStdString(cpuInfo.GetArchitecture()));
		_ui->lblVendorValue->setText(QString::fromStdString(cpuInfo.GetVendorName()));
		_ui->lblModelNameValue->setText(QString::fromStdString(cpuInfo.GetModelName()));
	}

	CpuInfoView::~CpuInfoView()
	{
		delete _ui;
	}
}