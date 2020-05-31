/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020  Ioannis Panagiotopoulos
 *
 *   This program is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program.  If not, see <https://www.gnu.org/licenses/>
 *************************************************************************/

#include "BenchmarkRunnerControlsView.hpp"
#include "ui_BenchmarkRunnerControlsView.h"

#include "Models/BenchmarkRunnerModel.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/StartBenchmarkingCommand.hpp"
#include "Core/Commands/StopBenchmarkingCommand.hpp"

namespace Elpida
{

	BenchmarkRunnerControlsView::BenchmarkRunnerControlsView(Mediator& mediator, const BenchmarkRunnerModel& model)
		: QWidget(), _ui(new Ui::BenchmarkRunnerControlsView), _model(model), _mediator(mediator), _running(false)
	{
		_ui->setupUi(this);


		QWidget::connect(this,
			&BenchmarkRunnerControlsView::onDataChanged,
			this,
			&BenchmarkRunnerControlsView::updateUi);
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
