#include "IntegerConfigurationView.hpp"
#include "ui_IntegerConfigurationView.h"

#include "Models/BenchmarkConfigurationInstanceModel.hpp"

namespace Elpida::Application
{

	IntegerConfigurationView::IntegerConfigurationView(QWidget* parent) :
		ConfigurationView(parent),
		_ui(new Ui::IntegerConfigurationView)
	{
		_ui->setupUi(this);
	}

	IntegerConfigurationView::~IntegerConfigurationView()
	{
		delete _ui;
	}

	void IntegerConfigurationView::on_spnValue_valueChanged(int value)
	{
		if (_model != nullptr)
		{
			_model->SetValue(std::to_string(value));
		}
	}

	void IntegerConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->lblName->setText(QString::fromStdString(_model->GetName()));
		}
	}

} // namespace Elpida
