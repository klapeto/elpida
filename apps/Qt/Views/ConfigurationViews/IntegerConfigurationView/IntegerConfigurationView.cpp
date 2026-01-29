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

#include "IntegerConfigurationView.hpp"
#include "ui_IntegerConfigurationView.h"

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{

	IntegerConfigurationView::IntegerConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::IntegerConfigurationView)
	{
		_ui->setupUi(this);
	}

	IntegerConfigurationView::~IntegerConfigurationView()
	{
		delete _ui;
	}

	void IntegerConfigurationView::on_spnValue_valueChanged(int value)
	{
		if (_model != nullptr  && !_settingModel)
		{
			_controller->SetValue(std::to_string(value));
		}
	}

	void IntegerConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
			_ui->spnValue->setValue(std::stol(_model->GetValue()));
		}
	}

} // namespace Elpida
