/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
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

#include "CommonDialog.hpp"
#include "ui_CommonDialog.h"

namespace Elpida
{

	CommonDialog::CommonDialog(QWidget* parent)
		:
		QDialog(parent),
		_ui(new Ui::CommonDialog)
	{
		_ui->setupUi(this);
	}

	CommonDialog::~CommonDialog()
	{
		delete _ui;
	}

	CommonDialog::DialogCode CommonDialog::show(QWidget* widget)
	{
		if (widget != nullptr)
		{
			_ui->verticalLayout->insertWidget(0, widget);
			auto result = (DialogCode)exec();
			_ui->verticalLayout->removeWidget(widget);
			return result;
		}
		return DialogCode::Rejected;
	}

} // namespace Elpida
