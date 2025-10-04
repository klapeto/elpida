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

#include "FloatConfigurationView.hpp"
#include "ui_FloatConfigurationView.h"

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"
#include "Elpida/Core/ValueUtilities.hpp"

namespace Elpida::Application
{

	FloatConfigurationView::FloatConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::FloatConfigurationView)
	{
		_ui->setupUi(this);
	}

	FloatConfigurationView::~FloatConfigurationView()
	{
		delete _ui;
	}

	void FloatConfigurationView::on_spnValue_valueChanged(double value)
	{
		auto toString = [](double d)
		{
			std::ostringstream doubleToStringAccumulator;
			doubleToStringAccumulator.imbue(std::locale::classic());
			doubleToStringAccumulator << d;
			return doubleToStringAccumulator.str();
		};

		if (_model != nullptr && !_settingModel)
		{
			_controller->SetValue(toString(value));
		}
	}

	void FloatConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
			_ui->spnValue->setValue(ValueUtilities::StringToDoubleInvariant(_model->GetValue()));
		}
	}

} // namespace Elpida
