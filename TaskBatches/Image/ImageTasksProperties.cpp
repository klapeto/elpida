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

#include "TaskBatches/Image/ImageTasksProperties.hpp"

#include "ui_ImageTasksProperties.h"
#include <QFileDialog>

ImageTasksProperties::ImageTasksProperties(QWidget *parent)
		: QWidget(parent), ui(new Ui::ImageTasksProperties)
{
	ui->setupUi(this);
}

ImageTasksProperties::~ImageTasksProperties()
{
	delete ui;
}

void ImageTasksProperties::on_pbSelectInput_clicked()
{
	auto filename = QFileDialog::getOpenFileName(this, "Open Png Image", "", "");
}
