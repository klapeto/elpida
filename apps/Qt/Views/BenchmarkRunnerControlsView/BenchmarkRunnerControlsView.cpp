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
#include "Models/GlobalConfigurationModel.hpp"
#include "Core/Abstractions/Mediator.hpp"
#include "Core/Commands/StartBenchmarkingCommand.hpp"
#include "Core/Commands/StopBenchmarkingCommand.hpp"

namespace Elpida
{

	BenchmarkRunnerControlsView::BenchmarkRunnerControlsView(Mediator& mediator,
			const BenchmarkRunnerModel& model,
			GlobalConfigurationModel& globalConfigurationModel)
			: QWidget(),
			  _ui(new Ui::BenchmarkRunnerControlsView),
			  _model(model),
			  _globalConfigurationModel(globalConfigurationModel),
			  _mediator(mediator)
	{
		_ui->setupUi(this);


		QWidget::connect(this,
				&BenchmarkRunnerControlsView::onDataChanged,
				this,
				&BenchmarkRunnerControlsView::updateUi);

		QWidget::connect(this,
				&BenchmarkRunnerControlsView::onConfigurationChanged,
				this,
				&BenchmarkRunnerControlsView::updateUi);

		_dataChangedEventSubscription = &_model.runningChanged.subscribe([this](auto running)
		{
			emit onDataChanged();
		});

		_configurationChangedEventSubscription = &_globalConfigurationModel.dataChanged.subscribe([this]
		{
			emit onConfigurationChanged();
		});

		QWidget::connect(_ui->pbRun, &QPushButton::clicked, this, &BenchmarkRunnerControlsView::startClicked);
		QWidget::connect(_ui->pbStop, &QPushButton::clicked, this, &BenchmarkRunnerControlsView::stopClicked);
		QWidget::connect(_ui->chkUploadResults,
				&QCheckBox::stateChanged,
				this,
				&BenchmarkRunnerControlsView::onUploadStateChanged);
		QWidget::connect(_ui->chkOpenWebPage,
				&QCheckBox::stateChanged,
				this,
				&BenchmarkRunnerControlsView::onOpenWebPageStateChanged);

		QWidget::connect(_ui->chkBlockUI,
				&QCheckBox::stateChanged,
				this,
				&BenchmarkRunnerControlsView::onBlockUIStateChanged);

		updateCheckboxes();
	}

	BenchmarkRunnerControlsView::~BenchmarkRunnerControlsView()
	{
		_dataChangedEventSubscription->unsubscribe();
		_configurationChangedEventSubscription->unsubscribe();
		delete _ui;
	}

	void BenchmarkRunnerControlsView::updateUi()
	{
		if (_model.isRunning())
		{
			_ui->pbRun->setEnabled(false);
			_ui->pbStop->setEnabled(true);
			_ui->spnTimes->setEnabled(false);
			_ui->chkBlockUI->setEnabled(false);
		}
		else
		{
			_ui->pbRun->setEnabled(true);
			_ui->pbStop->setEnabled(false);
			_ui->spnTimes->setEnabled(true);
			_ui->chkBlockUI->setEnabled(true);
		}

		updateCheckboxes();
	}

	void BenchmarkRunnerControlsView::updateCheckboxes() const
	{
		if (_globalConfigurationModel.isUploadResults())
		{
			_ui->chkUploadResults->setCheckState(Qt::Checked);
			_ui->chkOpenWebPage->setEnabled(true);
			_ui->chkOpenWebPage
					->setCheckState(_globalConfigurationModel.isOpenResultsWebPage() ? Qt::Checked : Qt::Unchecked);
		}
		else
		{
			_ui->chkUploadResults->setCheckState(Qt::Unchecked);
			_ui->chkOpenWebPage->setEnabled(false);
		}

		if (_globalConfigurationModel.isBlockUI())
		{
			_ui->chkBlockUI->setCheckState(Qt::Checked);
		}
	}

	void BenchmarkRunnerControlsView::startClicked(bool checked)
	{
		_mediator.execute(StartBenchmarkingCommand(_ui->spnTimes->value()));
	}

	void BenchmarkRunnerControlsView::stopClicked(bool checked)
	{
		_mediator.execute(StopBenchmarkingCommand());
	}

	void BenchmarkRunnerControlsView::onUploadStateChanged(int state)
	{
		_globalConfigurationModel.setUploadResults(state == 2);
		_ui->chkOpenWebPage->setEnabled(state == 2);
	}

	void BenchmarkRunnerControlsView::onOpenWebPageStateChanged(int state)
	{
		_globalConfigurationModel.setOpenResultsWebPage(state == 2);
	}

	void BenchmarkRunnerControlsView::onBlockUIStateChanged(int state)
	{
		_globalConfigurationModel.setBlockUI(state == 2);
	}

} // namespace Elpida
