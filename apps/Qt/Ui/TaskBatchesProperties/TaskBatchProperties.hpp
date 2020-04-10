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

#ifndef APPS_QT_UI_TASKBATCHPROPERTIES_TASKBATCHPROPERTIES_HPP
#define APPS_QT_UI_TASKBATCHPROPERTIES_TASKBATCHPROPERTIES_HPP

#include <QDialog>

namespace Elpida
{
	class QtTaskBatchWrapper;
	namespace Ui
	{
		class TaskBatchProperties;
	}  // namespace Ui

	class TaskBatchProperties : public QDialog
	{
	Q_OBJECT

	public:
		void setPage(QtTaskBatchWrapper* widget);

		void accept() override;

		explicit TaskBatchProperties(QWidget* parent = nullptr);
		~TaskBatchProperties() override;

	private:
		Ui::TaskBatchProperties* _ui;
		QtTaskBatchWrapper* _page;
	};

}  // namespace Elpida

#endif // APPS_QT_UI_TASKBATCHPROPERTIES_TASKBATCHPROPERTIES_HPP
