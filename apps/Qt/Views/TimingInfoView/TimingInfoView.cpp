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

#include "TimingInfoView.hpp"
#include "Elpida/Core/TimingInfo.hpp"
#include "ui_TimingInfoView.h"

#include "Models/SystemInfo/TimingModel.hpp"
#include "Elpida/Core/ValueUtilities.hpp"
#include <string>

using Vu = Elpida::ValueUtilities;

namespace Elpida::Application
{
	TimingInfoView::TimingInfoView(const TimingModel& model, QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::TimingInfoView)
	{
		_ui->setupUi(this);
		_ui->lblNowOverheadValue->setText(
			QString::fromStdString(Vu::Cs(Vu::GetValueScaleStringSI(model.GetNowOverhead().count()), "s")));
		_ui->lblLoopOverheadValue->setText(QString::fromStdString(
			Vu::Cs(Vu::GetValueScaleStringSI(model.GetLoopOverhead().count()), "s")));

		_ui->lblIpsValue->setText(QString::fromStdString(
			Vu::Cs(Vu::GetValueScaleStringSI(model.GetIterationsPerSecond()), "ips")));
	}

	TimingInfoView::~TimingInfoView()
	{
		delete _ui;
	}
}



