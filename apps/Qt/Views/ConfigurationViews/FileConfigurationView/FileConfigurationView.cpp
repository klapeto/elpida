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

#include "FileConfigurationView.hpp"
#include "ui_FileConfigurationView.h"

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"
#include <QFileDialog>

namespace Elpida::Application
{

	FileConfigurationView::FileConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::FileConfigurationView)
	{
		_ui->setupUi(this);
	}

	FileConfigurationView::~FileConfigurationView()
	{
		delete _ui;
	}

	void FileConfigurationView::on_pbOpen_clicked(bool checked)
	{
		QFileDialog dialog(this);
		dialog.setFileMode(QFileDialog::AnyFile);
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			_ui->leValue->setText(fileNames.at(0));
		}
	}

	void FileConfigurationView::on_leValue_textChanged(const QString& value)
	{
		if (_model != nullptr && !_settingModel)
		{
			_controller->SetValue(value.toStdString());
		}
	}
	void FileConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
			_ui->leValue->setText(QString::fromStdString(_model->GetValue()));
		}
	}

} // namespace Elpida
