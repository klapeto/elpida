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
#include <QCheckBox>

namespace Elpida
{

	ImageTasksProperties::ImageTasksProperties()
			: QtTaskBatchWrapper(), _ui(new Ui::ImageTasksProperties), _outputEnabled(false)
	{
		_ui->setupUi(this);
		_ui->leOutputImage->setEnabled(_outputEnabled);
		_ui->pbSelectOutput->setEnabled(_outputEnabled);
		_connections.push_back(
		        QCheckBox::connect(_ui->chkOutputImage, &QCheckBox::stateChanged, this,
		                           &ImageTasksProperties::on_chkOutputImage_stateChanged));
	}

	ImageTasksProperties::~ImageTasksProperties()
	{
		for (auto& connection : _connections)
		{
			QObject::disconnect(connection);
		}
		delete _ui;
	}

	void ImageTasksProperties::on_pbSelectInput_clicked()
	{
		auto filename = QFileDialog::getOpenFileName(this, "Open Png Image", "", "Png Images (*.png)");
		_inputImage = filename.toStdString();
		_ui->leInputImage->setText(filename);
	}

	void ImageTasksProperties::on_pbSelectOutput_clicked()
	{
		auto filename = QFileDialog::getSaveFileName(this, "Output Png Image", "", "Png Images (*.png)");
		_outputImage = filename.toStdString();
		_ui->leOutputImage->setText(filename);
	}

	void ImageTasksProperties::on_chkOutputImage_stateChanged(int state)
	{
		_outputEnabled = (bool) state;
		_ui->leOutputImage->setEnabled(_outputEnabled);
		_ui->pbSelectOutput->setEnabled(_outputEnabled);
	}

	void Elpida::ImageTasksProperties::reconfigureTaskBatch()
	{
		_taskBatch.setInputFile(_inputImage);
		_taskBatch.setOutputFile(_outputImage);
		_taskBatch.reconfigure();
	}

}  // namespace Elpida

