#include "FileConfigurationView.hpp"
#include "ui_FileConfigurationView.h"

#include "Models/ConfigurationModel.hpp"
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
		if (_model != nullptr)
		{
			_model->SetValue(value.toStdString());
		}
	}
	void FileConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
		}
	}

} // namespace Elpida
