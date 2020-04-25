#include "BenchmarkRunnerControlsView.hpp"
#include "ui_BenchmarkRunnerControlsView.h"

#include "Models/BenchmarkRunnerModel.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/StartBenchmarkingCommand.hpp"
#include "Core/Commands/StopBenchmarkingCommand.hpp"

namespace Elpida
{

	BenchmarkRunnerControlsView::BenchmarkRunnerControlsView(Mediator& mediator, const BenchmarkRunnerModel& model)
		: QWidget(), _ui(new Ui::BenchmarkRunnerControlsView), _model(model), _running(false), _mediator(mediator)
	{
		_ui->setupUi(this);


		QWidget::connect(this, &BenchmarkRunnerControlsView::onDataChanged, this, &BenchmarkRunnerControlsView::updateUi);
		_dataChangedEventSubscription = &_model.dataChanged.subscribe([this]
		{
			emit onDataChanged();
		});
		QWidget::connect(_ui->pbRun, &QPushButton::clicked, this, &BenchmarkRunnerControlsView::startClicked);
		QWidget::connect(_ui->pbStop, &QPushButton::clicked, this, &BenchmarkRunnerControlsView::stopClicked);
	}

	BenchmarkRunnerControlsView::~BenchmarkRunnerControlsView()
	{
		_dataChangedEventSubscription->unsubscribe();
		delete _ui;
	}

	void BenchmarkRunnerControlsView::updateUi()
	{
		if (_model.isRunning() && !_running)
		{
			_ui->pbRun->setEnabled(false);
			_ui->pbStop->setEnabled(true);
			_running = true;
		}
		else if (!_model.isRunning() && _running)
		{
			_ui->pbRun->setEnabled(true);
			_ui->pbStop->setEnabled(false);
			_running = false;
		}
	}
	void BenchmarkRunnerControlsView::startClicked(bool checked)
	{
		_mediator.execute(StartBenchmarkingCommand());
	}

	void BenchmarkRunnerControlsView::stopClicked(bool checked)
	{
		_mediator.execute(StopBenchmarkingCommand());
	}

} // namespace Elpida
