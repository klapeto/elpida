#include "TaskBatchRunnerStatusView.hpp"
#include "ui_TaskBatchRunnerStatusView.h"

namespace Elpida
{

	TaskBatchRunnerStatusView::TaskBatchRunnerStatusView(const TaskRunnerModel& model)
		: QWidget(), _ui(new Ui::TaskBatchRunnerStatusView), _model(model), _currentRunningTask(nullptr),
		  _currentRunningBenchmark(nullptr), _running(false)
	{
		_ui->setupUi(this);

		_runningString = "<span style=\" color:#b50000;\">Running</span>";
		_readyString = "<span style=\"color:#008d09;\">Ready</span>";
		_naString = "N/A";

		QWidget::connect(this, &TaskBatchRunnerStatusView::onDataUpdated, this, &TaskBatchRunnerStatusView::updateUi);
		_dataChangedEventSubscription = &_model.dataChanged.subscribe([this]
		{
			emit onDataUpdated();
		});
	}

	TaskBatchRunnerStatusView::~TaskBatchRunnerStatusView()
	{
		_dataChangedEventSubscription->unsubscribe();
		delete _ui;
	}

	void TaskBatchRunnerStatusView::updateUi()
	{
		if (_model.isRunning())
		{
			if (!_running)
			{
				_ui->lblSatusValue->setText(_runningString);
				_ui->pbTotalProgress->setMaximum(_model.getSessionTotalBenchmarksCount());
				_running = true;
			}
			auto task = _model.getCurrentRunningTask();
			if (task != nullptr && task != _currentRunningTask)
			{
				_ui->lblCurrentTaskName->setText(QString::fromStdString(task->getName()));
				_currentRunningTask = task;
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
			auto taskBatch = _model.getCurrentRunningBenchmark();
			if (taskBatch != nullptr && taskBatch != _currentRunningBenchmark)
			{
				_ui->lblCurrentTaskBatchName->setText(QString::fromStdString(taskBatch->getName()));
				_currentRunningBenchmark = taskBatch;
			}
		}
		else
		{
			_ui->lblSatusValue->setText(_readyString);
			_ui->lblCurrentTaskBatchName->setText(_naString);
			_ui->lblCurrentTaskName->setText(_naString);
			_currentRunningBenchmark = nullptr;
			_currentRunningTask = nullptr;
			_ui->pbBatchProgress->reset();
			_ui->pbTotalProgress->reset();
			_running = false;
		}
	}

} // namespace Elpida
