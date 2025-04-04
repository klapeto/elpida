#include "BooleanConfigurationView.hpp"
#include "ui_BooleanConfigurationView.h"

#include "Models/Benchmark/BenchmarkConfigurationModel.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{

	BooleanConfigurationView::BooleanConfigurationView(QWidget* parent)
			:
			ConfigurationView(parent),
			_ui(new Ui::BooleanConfigurationView)
	{
		_ui->setupUi(this);
	}

	BooleanConfigurationView::~BooleanConfigurationView()
	{
		delete _ui;
	}

	void BooleanConfigurationView::on_chkConfig_stateChanged(int state)
	{
		if (_model != nullptr && !_settingModel)
		{
			_controller->SetValue(state == Qt::Checked ? "true" : "false");
		}
	}

	void BooleanConfigurationView::OnModelSet()
	{
		if (_model != nullptr)
		{
			_ui->chkConfig->setText(QString::fromStdString(_model->GetName()));
			_ui->chkConfig->setChecked(_model->GetValue() == "true");
		}
	}

} // namespace Elpida
