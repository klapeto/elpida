#include "TaskBatchRunnerControlsView.hpp"
#include "ui_TaskBatchRunnerControlsView.h"

#include "Models/TaskRunnerModel.hpp"

namespace Elpida
{

	TaskBatchRunnerControlsView::TaskBatchRunnerControlsView(const TaskRunnerModel& model)
		: QWidget(), _ui(new Ui::TaskBatchRunnerControlsView), _model(model), _running(false)
	{
		_ui->setupUi(this);

		_dataChangedEventSubscription = &_model.dataChanged.subscribe([this]
		{
			onDataChanged();
		});
	}

	TaskBatchRunnerControlsView::~TaskBatchRunnerControlsView()
	{
		_dataChangedEventSubscription->unsubscribe();
		delete _ui;
	}

	void TaskBatchRunnerControlsView::onDataChanged()
	{
		if (_model.isRunning() && !_running)
		{
			_ui->pbRun->setEnabled(true);
			_ui->pbStop->setEnabled(false);
			_running = true;
		}
		else if (!_model.isRunning() && _running)
		{
			_ui->pbRun->setEnabled(false);
			_ui->pbStop->setEnabled(true);
			_running = true;
		}
	}

} // namespace Elpida
