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

#include "LogsDialog.hpp"
#include "ui_LogsDialog.h"

#include <QStandardItemModel>

namespace Elpida
{

	LogsDialog::LogsDialog(QWidget* parent, Logger& logger)
		: QDialog(parent),_ui(new Ui::LogsDialog)
	{
		_ui->setupUi(this);
		logger.addAppender(_logAppender);
		_ui->twLogs->setModel(&_logAppender.getModel());
		_ui->twLogs->resizeColumnToContents(1);
	}

	LogsDialog::~LogsDialog()
	{
		delete _ui;
	}

} // namespace Elpida
