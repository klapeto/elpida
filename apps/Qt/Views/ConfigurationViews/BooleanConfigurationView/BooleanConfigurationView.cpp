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

#include "BooleanConfigurationView.hpp"
#include "ui_BooleanConfigurationView.h"

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{

	BooleanConfigurationView::BooleanConfigurationView(QWidget* parent)
			:
			ConfigurationView(parent),
			_ui(new Ui::BooleanConfigurationView)
	{
		_ui->setupUi(this);
	}

	BooleanConfigurationView::~BooleanConfigurationView()
	{
		delete _ui;
	}

	void BooleanConfigurationView::on_chkConfig_stateChanged(int state)
	{
		if (_model != nullptr && !_settingModel)
		{
			_controller->SetValue(state == Qt::Checked ? "true" : "false");
		}
	}

	void BooleanConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->chkConfig->setText(QString::fromStdString(_model->GetName()));
			_ui->chkConfig->setChecked(_model->GetValue() == "true");
		}
	}

} // namespace Elpida
