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

#include "BenchmarkRunnerStatusView.hpp"
#include "ui_BenchmarkRunnerStatusView.h"

#include <Elpida/Engine/Task/TaskSpecification.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

#include "Models/BenchmarkRunnerModel.hpp"

namespace Elpida
{

	BenchmarkRunnerStatusView::BenchmarkRunnerStatusView(const BenchmarkRunnerModel& model)
		: QWidget(),
		  _ui(new Ui::BenchmarkRunnerStatusView),
		  _model(model),
		  _currentRunningTask(std::nullopt),
		  _currentRunningBenchmark(std::nullopt), _running(false)
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
		const auto& currentBenchmark = _model.getCurrentRunningBenchmark();
		if (currentBenchmark.has_value())
		{
			if (!_running)
			{
				_ui->lblSatusValue->setText(_runningString);
				_ui->pbBenchmarkProgress->setMaximum(currentBenchmark->get().getTotalTasksCount());
				_ui->lblCurrentBenchmarkName->setText(QString::fromStdString(currentBenchmark->get().getName()));
				_running = true;
			}

			auto currentRunningTask = _model.getCurrentRunningTask();
			if (currentRunningTask.has_value()
				&& (!_currentRunningTask.has_value()
					|| currentRunningTask->get().getId() != _currentRunningTask->get().getId()))
			{
				_ui->lblCurrentTaskName->setText(QString::fromStdString(currentRunningTask->get().getTaskSpecification().getName()));
				_currentRunningTask = currentRunningTask;
				_ui->pbTaskProgress->reset();
				_ui->pbTaskProgress->setMaximum(currentRunningTask->get().getIterationsToRun());
			}

			if (static_cast<size_t>(_ui->pbTaskProgress->value()) != _model.getTaskCompletedIterations())
			{
				_ui->pbTaskProgress->setValue(_model.getTaskCompletedIterations());
			}

			if (static_cast<size_t>(_ui->pbBenchmarkProgress->value()) != _model.getBenchmarkCompletedTasks())
			{
				_ui->pbBenchmarkProgress->setValue(_model.getBenchmarkCompletedTasks());
			}
		}
		else
		{
			_ui->lblSatusValue->setText(_readyString);
			_ui->lblCurrentBenchmarkName->setText(_naString);
			_ui->lblCurrentTaskName->setText(_naString);
			_currentRunningBenchmark = std::nullopt;
			_currentRunningTask = std::nullopt;
			_ui->pbBenchmarkProgress->reset();
			_ui->pbTaskProgress->reset();
			_running = false;
		}
	}

} // namespace Elpida
