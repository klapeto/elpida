#include "TaskBatchRunnerControlsView.hpp"
#include "ui_TaskBatchRunnerControlsView.h"

#include "Models/TaskRunnerModel.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/StartBenchmarkingCommand.hpp"
#include "Core/Commands/StopBenchmarkingCommand.hpp"

namespace Elpida
{

	TaskBatchRunnerControlsView::TaskBatchRunnerControlsView(Mediator& mediator, const TaskRunnerModel& model)
		: QWidget(), _ui(new Ui::TaskBatchRunnerControlsView), _model(model), _running(false), _mediator(mediator)
	{
		_ui->setupUi(this);

		_dataChangedEventSubscription = &_model.dataChanged.subscribe([this]
		{
			onDataChanged();
		});
		QWidget::connect(_ui->pbRun, &QPushButton::clicked, this, &TaskBatchRunnerControlsView::startClicked);
		QWidget::connect(_ui->pbStop, &QPushButton::clicked, this, &TaskBatchRunnerControlsView::stopClicked);
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
	void TaskBatchRunnerControlsView::startClicked(bool checked)
	{
		_mediator.execute(StartBenchmarkingCommand());
	}

	void TaskBatchRunnerControlsView::stopClicked(bool checked)
	{
		_mediator.execute(StopBenchmarkingCommand());
	}

} // namespace Elpida
