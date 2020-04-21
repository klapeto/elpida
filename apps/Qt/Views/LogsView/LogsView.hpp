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

#ifndef APPS_QT_UI_LOGSWIDGET_LOGSWIDGET_HPP
#define APPS_QT_UI_LOGSWIDGET_LOGSWIDGET_HPP

#include <QWidget>
#include <string>
#include <Elpida/Utilities/Logging/Logger.hpp>
#include "Core/QModelLogAppender.hpp"


namespace Elpida
{

	namespace Ui
	{
		class LogsView;
	}

	class LogsView final : public QWidget
	{
	Q_OBJECT

	public:
		explicit LogsView(Logger& logger);
		~LogsView() override;
	private:
		QModelLogAppender _logAppender;
		Ui::LogsView* _ui;
	};

} // namespace Elpida
#endif //APPS_QT_UI_LOGSWIDGET_LOGSWIDGET_HPP
