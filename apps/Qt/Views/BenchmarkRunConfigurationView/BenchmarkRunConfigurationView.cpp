#include "BenchmarkRunConfigurationView.hpp"
#include "ui_BenchmarkRunConfigurationView.h"

#include "Controllers/BenchmarkRunConfigurationController.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{
	BenchmarkRunConfigurationView::BenchmarkRunConfigurationView(const BenchmarkRunConfigurationModel& model,
			BenchmarkRunConfigurationController& controller, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::BenchmarkRunConfigurationView),
			_model(model),
			_controller(controller),
			_uiUpdating(false)
	{
		_ui->setupUi(this);
		UpdateUi();
		_modelChangedSubscription = _model.DataChanged().Subscribe([this]()
		{
			UpdateUi();
		});
	}

	BenchmarkRunConfigurationView::~BenchmarkRunConfigurationView()
	{
		delete _ui;
	}

	void BenchmarkRunConfigurationView::on_chkUpload_stateChanged(int state)
	{
		if (_uiUpdating) return;
		_controller.SetUploadResults(state == Qt::Checked);
	}

	void BenchmarkRunConfigurationView::on_chkOpenResult_stateChanged(int state)
	{
		if (_uiUpdating) return;
		_controller.SetOpenResultAfterUpload(state == Qt::Checked);
	}

	void BenchmarkRunConfigurationView::on_spnTimes_valueChanged(int value)
	{
		if (_uiUpdating) return;
		_controller.SetIterationsToRun(value);
	}

	void BenchmarkRunConfigurationView::UpdateUi()
	{
		if (_uiUpdating) return;
		_uiUpdating = true;
		_ui->chkUpload->setChecked(_model.IsUploadResults());
		_ui->chkOpenResult->setChecked(_model.IsOpenResult());
		_ui->chkNumaAware->setChecked(_model.IsNumaAware());
		_ui->chkOpenResult->setEnabled(_model.IsUploadResults());
		_ui->spnTimes->setValue(_model.GetIterationsToRun());
		_uiUpdating = false;
	}

	void BenchmarkRunConfigurationView::on_chkNumaAware_stateChanged(int state)
	{
		if (_uiUpdating) return;
		_controller.SetNumaAware(state == Qt::Checked);
	}
}