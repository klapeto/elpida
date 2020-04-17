#include "TaskBatchRunnerStatusView.hpp"
#include "ui_TaskBatchRunnerStatusView.h"

#include <Elpida/Task.hpp>
#include <Elpida/TaskBatch.hpp>

namespace Elpida
{

	TaskBatchRunnerStatusView::TaskBatchRunnerStatusView(const TaskRunnerModel& model)
		: QWidget(), _ui(new Ui::TaskBatchRunnerStatusView), _model(model), _currentRunningTask(nullptr),
		  _currentRunningTaskBatch(nullptr)
	{
		_ui->setupUi(this);

		_runningString = "<span style=\" color:#b50000;\">Running</span>";
		_readyString = "<span style=\"color:#008d09;\">Ready</span>";
		_naString = "N/A";

		_dataChangedEventSubscription = &_model.dataChanged.subscribe([this]
		{
			updateUi();
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
			if (_currentRunningTask == nullptr)
			{
				_ui->lblSatusValue->setText(_runningString);
				_ui->pbRunnerProgress->setMaximum(_model.getTotalTasksToRun());
			}
			auto task = _model.getCurrentRunningTask();
			if (task != nullptr && task != _currentRunningTask)
			{
				_ui->lblCurrentTaskName->setText(QString::fromStdString(task->getName()));
				_currentRunningTask = task;
				_ui->pbRunnerProgress->setValue(_model.getTasksThatRun());
			}
			auto taskBatch = _model.getCurrentRunningTaskBatch();
			if (taskBatch != nullptr && taskBatch != _currentRunningTaskBatch)
			{
				_ui->lblCurrentTaskBatchName->setText(QString::fromStdString(taskBatch->getName()));
				_currentRunningTaskBatch = taskBatch;
			}
		}
		else
		{
			_ui->lblSatusValue->setText(_readyString);
			_ui->lblCurrentTaskBatchName->setText(_naString);
			_ui->lblCurrentTaskName->setText(_naString);
			_currentRunningTaskBatch = nullptr;
			_currentRunningTask = nullptr;
			_ui->pbRunnerProgress->setValue(0);
		}
	}

} // namespace Elpida
