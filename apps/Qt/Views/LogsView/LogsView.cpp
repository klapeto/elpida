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

#include "LogsView.hpp"
#include "ui_LogsView.h"

#include <Elpida/Utilities/Logging/Logger.hpp>

namespace Elpida
{

	LogsView::LogsView(Logger& logger)
		: QWidget(), _ui(new Ui::LogsView)
	{
		_ui->setupUi(this);
		logger.addAppender(_logAppender);
		_ui->twLogs->setModel(&_logAppender.getModel());
		_ui->twLogs->resizeColumnToContents(1);

		connect(_ui->twLogs->selectionModel(), &QItemSelectionModel::selectionChanged,
				this, &LogsView::onSelectionChanged);

	}

	LogsView::~LogsView()
	{
		delete _ui;
	}

	void LogsView::onSelectionChanged(const QItemSelection& newSelection, const QItemSelection& oldSelection)
	{
		auto indexes = newSelection.front().indexes();
		if (indexes.size() < 3) return;
		_ui->txtTimestamp->setText(indexes[0].data(Qt::DisplayRole).toString());
		_ui->txtLevel->setText(indexes[1].data(Qt::DisplayRole).toString());
		_ui->txtMessage->setText(indexes[2].data(Qt::DisplayRole).toString());
		_ui->txtException->setPlainText(indexes[3].data(Qt::DisplayRole).toString());
	}

} // namespace Elpida