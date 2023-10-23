#include "FullBenchmarkView.hpp"

#include "ui_FullBenchmarkView.h"
#include "Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp"

#include "Models/Full/FullBenchmarkModel.hpp"
#include "Controllers/FullBenchmarkController.hpp"

namespace Elpida::Application
{
	using Vu = ValueUtilities;

	FullBenchmarkView::FullBenchmarkView(
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
			const FullBenchmarkModel& model,
			FullBenchmarkController& controller,
			QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::FullBenchmarkView), _model(model), _controller(controller), _currentBenchmarkIndex(0),
			_maxBenchmarkIndex(0), _cancel(false)
	{
		_ui->setupUi(this);
		dynamic_cast<QVBoxLayout*>(_ui->gbExecution->layout())->insertWidget(2,
				new BenchmarkRunConfigurationView(benchmarkRunConfigurationModel, benchmarkRunConfigurationController));

		_runningChanged = _model.RunningChanged().Subscribe([this](auto running)
		{
			if (running)
			{
				_currentBenchmarkIndex = 0;
				_maxBenchmarkIndex = _model.GetTotalBenchmarks();
				_ui->pbProgress->setRange(0, _maxBenchmarkIndex);
				_ui->bpStart->setText("Cancel");
			}
			else
			{
				_currentBenchmarkIndex = 0;
				_maxBenchmarkIndex = 0;
				_ui->bpStart->setText("Start");
				if(!_cancel)
				{
					UpdateScore();
				}
			}
			UpdateProgress();
			_cancel = false;
			_ui->lblCurrentBenchmarkValue->setText("N/A");
		});

		_currentBenchmarkChanged = _model.CurrentRunningBenchmarkChanged().Subscribe([this](auto& benchmark)
		{
			_ui->lblCurrentBenchmarkValue->setText(QString::fromStdString(benchmark));
			UpdateProgress();
			_currentBenchmarkIndex++;
		});
	}

	FullBenchmarkView::~FullBenchmarkView()
	{
		delete _ui;
	}

	void FullBenchmarkView::UpdateProgress()
	{
		_ui->lblProgress->setText(QString::fromStdString(
				std::to_string(_currentBenchmarkIndex) + "/" + std::to_string(_maxBenchmarkIndex)));
		_ui->pbProgress->setValue(_currentBenchmarkIndex);
	}

	void FullBenchmarkView::UpdateScore()
	{
		auto& results = _model.GetResults();
		if (results.empty()) return;
		auto& currentResult = results.back();

		_ui->lblTotalScore->setText(QString::fromStdString(std::to_string(currentResult.GetTotalScore())));
		_ui->lblSingleCoreScoreValue->setText(
				QString::fromStdString(std::to_string(currentResult.GetSingleCoreScore())));
		_ui->lblMultiCoreScoreValue->setText(QString::fromStdString(std::to_string(currentResult.GetMultiCoreScore())));
		_ui->lblMemoryScoreValue->setText(QString::fromStdString(std::to_string(currentResult.GetMemoryScore())));

		if (results.size() > 1)
		{
			auto& previousResult = results[results.size() - 2];
			auto delta = (currentResult.GetTotalScore() / previousResult.GetTotalScore()) * 100;
			_ui->lblTotalScoreDelta->setText(QString::fromStdString(std::to_string(delta)));
			_ui->lblTotalScoreDelta->setStyleSheet(delta >= 0.0 ? "color: green" : "color: red");
		}
	}

	void FullBenchmarkView::on_bpStart_clicked(bool checked)
	{
		if (_model.IsRunning())
		{
			_controller.StopRunning();
		}
		else
		{
			_controller.RunAsync();
		}
	}
}