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
		  _currentRunningTaskSpecification(std::nullopt),
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
		if (_model.isRunning())
		{
			if (!_running)
			{
				_ui->lblSatusValue->setText(_runningString);
				_ui->pbTotalProgress->setMaximum(_model.getSessionTotalBenchmarksCount());
				_running = true;
			}

			auto taskSpec = _model.getCurrentRunningTaskSpecification();
			if (taskSpec.has_value() && (!_currentRunningTaskSpecification.has_value() || &taskSpec->get() != &_currentRunningTaskSpecification->get()))
			{
				_ui->lblCurrentTaskName->setText(QString::fromStdString(taskSpec->get().getName()));
				_currentRunningTaskSpecification = taskSpec;
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
			if (benchmark.has_value() && (!_currentRunningBenchmark.has_value() || &benchmark->get() != &_currentRunningBenchmark->get()))
			{
				_ui->lblCurrentTaskBatchName->setText(QString::fromStdString(benchmark->get().getName()));
				_currentRunningBenchmark = benchmark;
			}
		}
		else
		{
			_ui->lblSatusValue->setText(_readyString);
			_ui->lblCurrentTaskBatchName->setText(_naString);
			_ui->lblCurrentTaskName->setText(_naString);
			_currentRunningBenchmark = std::nullopt;
			_currentRunningTaskSpecification = std::nullopt;
			_ui->pbBatchProgress->reset();
			_ui->pbTotalProgress->reset();
			_running = false;
		}
	}

} // namespace Elpida
