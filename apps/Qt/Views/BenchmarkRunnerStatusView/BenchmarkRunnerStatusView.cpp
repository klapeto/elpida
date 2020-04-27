#include "BenchmarkRunnerStatusView.hpp"
#include "ui_BenchmarkRunnerStatusView.h"

#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

namespace Elpida
{

	BenchmarkRunnerStatusView::BenchmarkRunnerStatusView(const BenchmarkRunnerModel& model)
		: QWidget(), _ui(new Ui::BenchmarkRunnerStatusView), _model(model), _currentRunningTaskSpecification(nullptr),
		  _currentRunningBenchmark(nullptr), _running(false)
	{
		_ui->setupUi(this);

		_runningString = "<span style=\" color:#b50000;\">Running</span>";
		_readyString = "<span style=\"color:#008d09;\">Ready</span>";
		_naString = "N/A";

		QWidget::connect(this, &BenchmarkRunnerStatusView::onDataUpdated, this, &BenchmarkRunnerStatusView::updateUi);
		_dataChangedEventSubscription = &_model.dataChanged.subscribe([this]
		{
			emit onDataUpdated();
		});
	}

	BenchmarkRunnerStatusView::~BenchmarkRunnerStatusView()
	{
		_dataChangedEventSubscription->unsubscribe();
		delete _ui;
	}

	void BenchmarkRunnerStatusView::updateUi()
	{
		if (_model.isRunning())
		{
			if (!_running)
			{
				_ui->lblSatusValue->setText(_runningString);
				_ui->pbTotalProgress->setMaximum(_model.getSessionTotalBenchmarksCount());
				_running = true;
			}
			auto task = _model.getCurrentRunningTaskSpecification();
			if (task != nullptr && task != _currentRunningTaskSpecification)
			{
				_ui->lblCurrentTaskName->setText(QString::fromStdString(task->getName()));
				_currentRunningTaskSpecification = task;
				_ui->pbBatchProgress->reset();
				_ui->pbBatchProgress->setMaximum(_model.getBatchTotalTasksCount());
			}
			if (static_cast<size_t>(_ui->pbBatchProgress->value()) != _model.getBatchExecutedTasksCount())
			{
				_ui->pbBatchProgress->setValue(_model.getBatchExecutedTasksCount());
			}
			if (static_cast<size_t>(_ui->pbTotalProgress->value()) != _model.getSessionCompletedBenchmarksCount())
			{
				_ui->pbTotalProgress->setValue(_model.getSessionCompletedBenchmarksCount());
			}
			auto benchmark = _model.getCurrentRunningBenchmark();
			if (benchmark != nullptr && benchmark != _currentRunningBenchmark)
			{
				_ui->lblCurrentTaskBatchName->setText(QString::fromStdString(benchmark->getName()));
				_currentRunningBenchmark = benchmark;
			}
		}
		else
		{
			_ui->lblSatusValue->setText(_readyString);
			_ui->lblCurrentTaskBatchName->setText(_naString);
			_ui->lblCurrentTaskName->setText(_naString);
			_currentRunningBenchmark = nullptr;
			_currentRunningTaskSpecification = nullptr;
			_ui->pbBatchProgress->reset();
			_ui->pbTotalProgress->reset();
			_running = false;
		}
	}

} // namespace Elpida
