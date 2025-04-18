#include "BenchmarkRunConfigurationView.hpp"
#include "ui_BenchmarkRunConfigurationView.h"

#include "Controllers/BenchmarkRunConfigurationController.hpp"
#include "Models/BenchmarkRunConfigurationModel.hpp"

namespace Elpida::Application
{
	static int TranslateConcurrencyMode(ConcurrencyMode concurrencyMode){
		switch (concurrencyMode)
		{
		case ConcurrencyMode::None:
			return 0;
		case ConcurrencyMode::CopyInput:
			return 1;
		case ConcurrencyMode::ShareInput:
			return 2;
		case ConcurrencyMode::ChunkInput:
			return 3;
		}
		return 0;
	}

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
		_ui->chkPinThreads->setChecked(_model.IsPinThreads());
		_ui->chkOpenResult->setEnabled(_model.IsUploadResults());
		_ui->spnTimes->setValue(_model.GetIterationsToRun());
		_ui->spnDelay->setValue(_model.GetDelaySecondsBetweenRuns());
		_ui->chkGenerateReport->setChecked(_model.IsGenerateHtmlReport());
		_ui->cmbConcurrencyMode->setCurrentIndex(TranslateConcurrencyMode(_model.GetConcurrencyMode()));
		_ui->spnMinimumMicroTaskTime->setValue(_model.GetMinimumMicroTaskDuration().count());
		_uiUpdating = false;
	}

	void BenchmarkRunConfigurationView::on_chkNumaAware_stateChanged(int state)
	{
		if (_uiUpdating) return;
		_controller.SetNumaAware(state == Qt::Checked);
	}

	void BenchmarkRunConfigurationView::on_chkPinThreads_stateChanged(int state)
	{
		if (_uiUpdating) return;
		_controller.SetPinThreads(state == Qt::Checked);
	}

	void BenchmarkRunConfigurationView::DisableAdvancedSettings()
	{
		_ui->gbAdvanced->setVisible(false);
	}

	void BenchmarkRunConfigurationView::on_cmbConcurrencyMode_currentIndexChanged(int value)
	{
		if (_uiUpdating) return;
		switch (value)
		{
		case 1:
			_controller.SetConcurrencyMode(ConcurrencyMode::CopyInput);
			break;
		case 2:
			_controller.SetConcurrencyMode(ConcurrencyMode::ShareInput);
			break;
		case 3:
			_controller.SetConcurrencyMode(ConcurrencyMode::ChunkInput);
			break;
		default:
			_controller.SetConcurrencyMode(ConcurrencyMode::None);
		}
	}

	void BenchmarkRunConfigurationView::on_chkGenerateReport_stateChanged(int state)
	{
		if (_uiUpdating) return;
		_controller.SetGenerateHtmlReport(state == Qt::Checked);
	}

	void BenchmarkRunConfigurationView::DisableUpload()
	{
		_ui->chkOpenResult->setVisible(false);
		_ui->chkUpload->setVisible(false);
	}

	void BenchmarkRunConfigurationView::DisableIterations()
	{
		_ui->lblRun->setVisible(false);
		_ui->lblTimes->setVisible(false);
		_ui->spnTimes->setVisible(false);
	}

	void BenchmarkRunConfigurationView::DisableHtmlReport()
	{
		_ui->chkGenerateReport->setVisible(false);
	}

	void BenchmarkRunConfigurationView::on_spnDelay_valueChanged(int value)
	{
		if (_uiUpdating) return;
		_controller.SetDelaySecondsBetweenRuns(value);
	}

	void BenchmarkRunConfigurationView::DisableDelay()
	{
		_ui->spnDelay->setVisible(false);
		_ui->lblDelay->setVisible(false);
	}

	void BenchmarkRunConfigurationView::on_spnMinimumMicroTaskTime_valueChanged(double value)
	{
		if (_uiUpdating) return;
		_controller.SetMinimumMicroTaskDuration(Duration (value));
	}
}