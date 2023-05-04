#include "StringConfigurationView.hpp"
#include "ui_StringConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{

	StringConfigurationView::StringConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::StringConfigurationView)
	{
		_ui->setupUi(this);
	}

	StringConfigurationView::~StringConfigurationView()
	{
		delete _ui;
	}

	void StringConfigurationView::on_leValue_textChanged(const QString& value)
	{
		if (_model != nullptr)
		{
			_controller->SetValue(value.toStdString());
		}
	}

	void StringConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
		}
	}

} // namespace Elpida