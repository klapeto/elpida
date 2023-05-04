#include "FloatConfigurationView.hpp"
#include "ui_FloatConfigurationView.h"

#include "Models/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{

	FloatConfigurationView::FloatConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::FloatConfigurationView)
	{
		_ui->setupUi(this);
	}

	FloatConfigurationView::~FloatConfigurationView()
	{
		delete _ui;
	}

	void FloatConfigurationView::on_spnValue_valueChanged(double value)
	{
		if (_model != nullptr)
		{
			_controller->SetValue(std::to_string(value));
		}
	}

	void FloatConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
			_ui->spnValue->setValue(std::stod(_model->GetValue()));
		}
	}

} // namespace Elpida