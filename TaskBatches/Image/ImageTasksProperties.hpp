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

#ifndef IMAGETASKSPROPERIES_HPP
#define IMAGETASKSPROPERIES_HPP

#include <QWidget>

namespace Ui
{
	class ImageTasksProperties;
}

class ImageTasksProperties: public QWidget
{
	Q_OBJECT

	public:
		explicit ImageTasksProperties(QWidget *parent = 0);
		~ImageTasksProperties();

	private slots:
		void on_pbSelectInput_clicked();

	private:
		Ui::ImageTasksProperties *ui;
};

#endif // IMAGETASKSPROPERIES_HPP
