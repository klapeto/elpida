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

#include "StringConfigurationView.hpp"
#include "ui_StringConfigurationView.h"

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{

	StringConfigurationView::StringConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::StringConfigurationView)
	{
		_ui->setupUi(this);
	}

	StringConfigurationView::~StringConfigurationView()
	{
		delete _ui;
	}

	void StringConfigurationView::on_leValue_textChanged(const QString& value)
	{
		if (_model != nullptr && !_settingModel)
		{
			_controller->SetValue(value.toStdString());
		}
	}

	void StringConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
			_ui->leValue->setText(QString::fromStdString(_model->GetValue()));
		}
	}

} // namespace Elpida
