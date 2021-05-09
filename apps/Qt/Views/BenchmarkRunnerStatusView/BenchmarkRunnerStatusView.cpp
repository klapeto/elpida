/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020 Ioannis Panagiotopoulos
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
			  _model(model)
	{
		_ui->setupUi(this);

		_runningString = "<span style=\" color:#b50000;\">Running</span>";
		_readyString = "<span style=\"color:#008d09;\">Ready</span>";
		_naString = "N/A";
		_zeroString = "0";

		QWidget::connect(this, &BenchmarkRunnerStatusView::onCompletedBenchmarkTasksChanged, this,
				&BenchmarkRunnerStatusView::updateCompletedBenchmarkTasksChanged);
		QWidget::connect(this, &BenchmarkRunnerStatusView::onCompletedBenchmarksChanged, this,
				&BenchmarkRunnerStatusView::updateCompletedBenchmarksChanged);
		QWidget::connect(this, &BenchmarkRunnerStatusView::onCompletedTaskIterationsChanged, this,
				&BenchmarkRunnerStatusView::updateCompletedTaskIterationsChanged);
		QWidget::connect(this, &BenchmarkRunnerStatusView::onCurrentRunningBenchmarkChanged, this,
				&BenchmarkRunnerStatusView::updateCurrentRunningBenchmarkChanged);
		QWidget::connect(this, &BenchmarkRunnerStatusView::onCurrentRunningTaskChanged, this,
				&BenchmarkRunnerStatusView::updateCurrentRunningTaskChanged);
		QWidget::connect(this, &BenchmarkRunnerStatusView::onTotalBenchmarksChanged, this,
				&BenchmarkRunnerStatusView::updateTotalBenchmarksChanged);
		QWidget::connect(this, &BenchmarkRunnerStatusView::onRunningChanged, this,
				&BenchmarkRunnerStatusView::updateRunningChanged);

		_subscriptions.emplace_back(_model.completedBenchmarkTasksChanged.subscribe([this](auto x)
		{ emit onCompletedBenchmarkTasksChanged(); }));
		_subscriptions.emplace_back(_model.completedBenchmarksChanged.subscribe([this](auto x)
		{ emit onCompletedBenchmarksChanged(); }));
		_subscriptions.emplace_back(_model.completedTaskIterationsChanged.subscribe([this](auto x)
		{ emit onCompletedTaskIterationsChanged(); }));
		_subscriptions.emplace_back(_model.currentRunningBenchmarkChanged.subscribe([this](auto b)
		{ emit onCurrentRunningBenchmarkChanged(); }));
		_subscriptions.emplace_back(_model.currentRunningTaskChanged.subscribe([this](auto b)
		{ emit onCurrentRunningTaskChanged(); }));
		_subscriptions.emplace_back(_model.totalBenchmarksChanged.subscribe([this](auto x)
		{ emit onTotalBenchmarksChanged(); }));
		_subscriptions.emplace_back(_model.runningChanged.subscribe([this](auto x)
		{ emit onRunningChanged(); }));
	}

	BenchmarkRunnerStatusView::~BenchmarkRunnerStatusView()
	{
		for (auto& subscription: _subscriptions)
		{
			subscription.get().unsubscribe();
		}
		delete _ui;
	}

	void BenchmarkRunnerStatusView::updateCurrentRunningTaskChanged()
	{
		_ui->pbTaskProgress->reset();
		_ui->lblIterationCurrentValue->setText(_zeroString);

		auto newTask = _model.getCurrentRunningTask();

		if (newTask.has_value())
		{
			auto& task = newTask->get();
			_ui->pbTaskProgress->setMaximum(task.getIterationsToRun());
			_ui->lblCurrentTaskName->setText(QString::fromStdString(task.getTaskSpecification().getName()));
			_ui->lblIterationMaxValue->setText(QString::fromStdString(std::to_string(task.getIterationsToRun())));
		}
		else
		{
			_ui->lblIterationMaxValue->setText(_zeroString);
			_ui->lblCurrentTaskName->setText(_naString);
		}
	}

	void BenchmarkRunnerStatusView::updateCurrentRunningBenchmarkChanged()
	{
		_ui->pbBenchmarkProgress->reset();
		_ui->lblBenchmarkTaskCurrentValue->setText(_zeroString);

		auto newBenchmark = _model.getCurrentRunningBenchmark();

		if (newBenchmark.has_value())
		{
			auto& benchmark = newBenchmark->get();
			_ui->pbBenchmarkProgress->setMaximum(benchmark.getTotalTasksCount());
			_ui->lblCurrentBenchmarkName->setText(QString::fromStdString(benchmark.getName()));
			_ui->lblBenchmarkTaskMaxValue->setText(
					QString::fromStdString(std::to_string(benchmark.getTotalTasksCount())));
		}
		else
		{
			_ui->lblBenchmarkTaskMaxValue->setText(_zeroString);
			_ui->lblCurrentBenchmarkName->setText(_naString);
		}
	}

	void BenchmarkRunnerStatusView::updateCompletedBenchmarkTasksChanged()
	{
		auto completedTasks = _model.getBenchmarkCompletedTasks();
		_ui->lblBenchmarkTaskCurrentValue->setText(QString::fromStdString(std::to_string(completedTasks)));
		_ui->pbBenchmarkProgress->setValue(completedTasks);
	}

	void BenchmarkRunnerStatusView::updateCompletedTaskIterationsChanged()
	{
		auto completedIterations = _model.getTaskCompletedIterations();
		_ui->lblIterationCurrentValue->setText(QString::fromStdString(std::to_string(completedIterations)));
		_ui->pbTaskProgress->setValue(completedIterations);
	}

	void BenchmarkRunnerStatusView::updateCompletedBenchmarksChanged()
	{
		auto completedBenchmarks = _model.getCompletedBenchmarks();
		_ui->lblBenchmarksCurrentValue->setText(QString::fromStdString(std::to_string(completedBenchmarks)));
		_ui->pbTotalProgress->setValue(completedBenchmarks);
	}

	void BenchmarkRunnerStatusView::updateTotalBenchmarksChanged()
	{
		auto totalBenchmarks = _model.getTotalBenchmarks();
		if (totalBenchmarks > 0)
		{
			_ui->lblBenchmarksCurrentValue->setText(_zeroString);
			_ui->lblBenchmarksMaxValue->setText(QString::fromStdString(std::to_string(totalBenchmarks)));
			_ui->pbTotalProgress->setMaximum(totalBenchmarks);
			_ui->pbTotalProgress->setValue(0);
		}
		else
		{
			// This is required to prevent progress bar weird stuff
			_ui->lblBenchmarksCurrentValue->setText(_zeroString);
			_ui->lblBenchmarksMaxValue->setText(_zeroString);
			_ui->pbTotalProgress->setMaximum(1);
			_ui->pbTotalProgress->setValue(0);
		}
	}

	void BenchmarkRunnerStatusView::updateRunningChanged()
	{
		if (_model.isRunning())
		{
			_ui->lblSatusValue->setText(_runningString);
		}
		else
		{
			_ui->lblSatusValue->setText(_readyString);
		}
	}

} // namespace Elpida
