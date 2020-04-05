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

#ifndef ELPIDA_LOGSDIALOG_HPP
#define ELPIDA_LOGSDIALOG_HPP

#include <qdialog.h>
#include <qobjectdefs.h>
#include <string>

namespace Elpida
{

	namespace Ui
	{
		class LogsDialog;
	}

	class LogsDialog final : public QDialog
	{
	Q_OBJECT

	public:
		void setLogsText(const std::string& text);

		explicit LogsDialog(QWidget* parent = 0);
		~LogsDialog();

	private:
		Ui::LogsDialog* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_LOGSDIALOG_HPP
