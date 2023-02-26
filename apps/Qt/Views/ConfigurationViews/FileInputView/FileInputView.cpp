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

#include "FileInputView.hpp"
#include "ui_FileInputView.h"

#include <Elpida/Engine/Configuration/Specification/ConfigurationSpecificationBase.hpp>
#include "Elpida/Common/ElpidaException.hpp"
#include <Elpida/Config.hpp>

#include <QFileDialog>

namespace Elpida
{

	FileInputView::FileInputView()
		: ConfigurationValueViewBase(), _ui(new Ui::FileInputView), _configurationValue(nullptr)
	{
		_ui->setupUi(this);
		QWidget::connect(_ui->leFilePath, &QLineEdit::editingFinished, this, &FileInputView::onEditingFinished);
		QWidget::connect(_ui->pbBrowse, &QPushButton::clicked, this, &FileInputView::onBrowseClicked);
	}

	FileInputView::~FileInputView()
	{
		delete _ui;
	}

	void FileInputView::setConfiguration(const std::shared_ptr<ConfigurationValueBase>& configurationValue)
	{
		if (configurationValue)
		{
			auto& spec = configurationValue->getConfigurationSpecification();
			if (spec.getType() == ConfigurationType::Type::FilePath)
			{
				_configurationValue = configurationValue;
				auto& value = _configurationValue->as<ConfigurationValue<std::string>>();
				_ui->lblPropertyName->setText(QString::fromStdString(spec.getName()));
				_ui->leFilePath->setText(QString::fromStdString(value.getValue()));
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME, "Invalid Configuration! Expected FilePath configuration");
			}
		}
		else
		{
			_configurationValue.reset();
		}
	}

	void FileInputView::saveSetting()
	{
		if (_configurationValue != nullptr)
		{
			_configurationValue->as<ConfigurationValue<std::string>>().setValue(_ui->leFilePath->text().toStdString());
		}
	}

	void FileInputView::onEditingFinished()
	{
		saveSetting();
	}

	void FileInputView::onBrowseClicked(bool checked)
	{
		QFileDialog dialog(this);
		dialog.setFileMode(QFileDialog::AnyFile);
		QStringList fileNames;
		if (dialog.exec())
		{
			fileNames = dialog.selectedFiles();
			_ui->leFilePath->setText(fileNames.at(0));
			saveSetting();
		}
	}

} // namespace Elpida
