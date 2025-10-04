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

#include "MemoryInfoView.hpp"
#include "ui_MemoryInfoView.h"

#include "Models/SystemInfo/MemoryInfoModel.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	MemoryInfoView::MemoryInfoView(const MemoryInfoModel& memoryInfo, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::MemoryInfoView)
	{
		_ui->setupUi(this);

		_ui->lblTotalSizeValue->setText(
				QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringIEC(memoryInfo.GetTotalSize()), "B")));
		_ui->lblPageSizeValue->setText(
				QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringIEC(memoryInfo.GetPageSize()), "B")));
	}

	MemoryInfoView::~MemoryInfoView()
	{
		delete _ui;
	}
}