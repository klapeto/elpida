#include "FileInputView.hpp"
#include "ui_FileInputView.h"

#include <Elpida/Engine/Configuration/ConfigurationValue.hpp>
#include <Elpida/Engine/Configuration/ConfigurationSpecificationBase.hpp>
#include <Elpida/ElpidaException.hpp>
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

	void FileInputView::setConfiguration(ConfigurationValueBase* configurationValue)
	{
		if (configurationValue != nullptr)
		{
			auto& spec = configurationValue->getConfigurationSpecification();
			if (spec.getType() == ConfigurationType::Type::FilePath)
			{
				auto& value = configurationValue->as<ConfigurationValue<std::string>>();
				_configurationValue = &value;
				_ui->lblPropertyName->setText(QString::fromStdString(spec.getName()));
				_ui->leFilePath->setText(QString::fromStdString(value.getValue()));
			}
			else
			{
				throw ElpidaException(FUNCTION_NAME, "Invalid Configuration! Expected FilePath configuration");
			}
		}
	}

	ConfigurationValueBase* FileInputView::getConfiguration()
	{
		return _configurationValue;
	}

	void FileInputView::saveSetting()
	{
		if (_configurationValue != nullptr)
		{
			_configurationValue->setValue(_ui->leFilePath->text().toStdString());
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
