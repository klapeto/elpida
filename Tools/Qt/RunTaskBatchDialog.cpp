/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2018  Ioannis Panagiotopoulos
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

#include "ui_RunTaskBatchDialog.h"
#include "RunTaskBatchDialog.hpp"
#include <Elpida/TaskBatch.hpp>
#include <Elpida/Types/String.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include <QMessageBox>
#include <QTreeWidgetItem>

namespace Elpida
{


	RunTaskBatchDialog::RunTaskBatchDialog(const Map<String, QtTaskBatchWrapper*>& taskBatchList, QWidget *parent)
			: QDialog(parent), _taskBatchList(taskBatchList), _ui(new Ui::RunTaskBatchDialog)
	{
		_runningText = "<span style=\" color:#b50000;\">Running</span>";
		_readyText = "<span style=\"color:#008d09;\">Ready</span>";
		_na = "N/A";
		_ui->setupUi(this);
		onTaskBatchListModified();


		QPushButton::connect(this, &RunTaskBatchDialog::onTaskBatchStart, this, &RunTaskBatchDialog::updateForTaskBatchBegin);
		QPushButton::connect(this, &RunTaskBatchDialog::onTaskStart, this, &RunTaskBatchDialog::updateForTaskBegin);
		QPushButton::connect(this, &RunTaskBatchDialog::onTaskBatchEnd, this, &RunTaskBatchDialog::updateForTaskBatchEnd);
		QPushButton::connect(this, &RunTaskBatchDialog::onSessionBegin, this, &RunTaskBatchDialog::updateForSessionBegin);
		QPushButton::connect(this, &RunTaskBatchDialog::onSessionEnd, this, &RunTaskBatchDialog::updateForSessionEnd);

		_taskBatchRunner.taskStart.subscribe([this](const Runner::EventArguments::TaskStart& args) { emit onTaskStart(QString::fromStdString(args.name));});
		_taskBatchRunner.batchStart.subscribe([this](const Runner::EventArguments::BatchStart& args) { emit onTaskBatchStart(QString::fromStdString(args.name), args.numberOfTasks);});
		_taskBatchRunner.batchEnd.subscribe([this](const Runner::EventArguments::BatchEnd& args) { emit onTaskBatchEnd(QString::fromStdString(args.name));});
	}

	RunTaskBatchDialog::~RunTaskBatchDialog()
	{
		delete _ui;
	}

	void RunTaskBatchDialog::onTaskBatchListModified()
	{
		int childrenSize = _ui->lwTaskBatches->count(), c = 0;
		for (auto& taskBatch : _taskBatchList)
		{
			auto item = _ui->lwTaskBatches->item(c++);
			if (item != nullptr)
			{
				((QListWidgetItem*) item)->setText(QString::fromStdString(taskBatch.second->getTaskBatch().getName()));

			}
			else
			{
				new QListWidgetItem(QString::fromStdString(taskBatch.second->getTaskBatch().getName()), _ui->lwTaskBatches);
			}
		}

		if (c < childrenSize)
		{
			for (int i = childrenSize; i > c; --i)
			{
				_ui->lwTaskBatches->removeItemWidget(_ui->lwTaskBatches->item(i));
			}
		}

	}

	void RunTaskBatchDialog::appendResults()
	{
		auto& results = _taskBatchRunner.getLastExecutionResults();
		for (auto& batchResult : results)
		{
			auto batchItem = new QTreeWidgetItem(_ui->twResults);
			batchItem->setText(0, QString::fromStdString(batchResult.first));
			for (auto& taskResults : batchResult.second)
			{
				auto taskItem = new QTreeWidgetItem(batchItem);
				taskItem->setText(0, QString::fromStdString(taskResults.first));
				if (taskResults.second.size() == 1)
				{
					taskItem->setText(1, QString::fromStdString(taskResults.second[0].getPerSecond()));
				}
				else
				{
					for (auto& taskResult : taskResults.second)
					{
						auto taskResultItem = new QTreeWidgetItem(taskItem);
						taskResultItem->setText(0, QString::fromStdString(taskResult.getRunResult().getResultDescription()));
						taskResultItem->setText(1, QString::fromStdString(taskResult.getPerSecond()));
					}
				}
			}
		}
	}

	void RunTaskBatchDialog::updateForTaskBatchBegin(const QString& name, int size)
	{
		_ui->taskBatchRunProgress->setMaximum(size);
		_ui->lblCurrentTaskBatchName->setText(name);
	}

	void RunTaskBatchDialog::updateForTaskBegin(const QString& name)
	{
		_ui->taskBatchRunProgress->setValue(_ui->taskBatchRunProgress->value() + 1);
		_ui->lblCurrentTaskName->setText(name);
	}

	void RunTaskBatchDialog::updateForTaskBatchEnd(const QString& name)
	{
		_ui->taskBatchRunProgress->setValue(0);
	}

	void RunTaskBatchDialog::updateForSessionBegin()
	{
		_ui->lblSatusValue->setText(_runningText);
		_ui->pbStop->setEnabled(true);
	}

	void RunTaskBatchDialog::updateForSessionEnd()
	{
		_ui->lblSatusValue->setText(_readyText);
		_ui->lblCurrentTaskName->setText(_na);
		_ui->lblCurrentTaskBatchName->setText(_na);
		_ui->pbStop->setEnabled(false);
		appendResults();
	}

	void RunTaskBatchDialog::runTaskBatches()
	{
		try
		{
			_taskBatchRunner.executeTasks();
		}
		catch (ElpidaException& e)
		{
			QMessageBox::critical(_ui->pbRun, "Error", QString::fromStdString("Task batch runner produced error: " + e.getMessage()),
			                      QMessageBox::StandardButton::Ok);
		}
	}

	void RunTaskBatchDialog::on_pbRun_clicked()
	{
		if (!_taskRunnerThread.isRunning())
		{
			_ui->lblSatusValue->setText(_runningText);
			_ui->pbStop->setEnabled(true);
			_taskBatchRunner.clearTaskBatches();
			auto selectedItems = _ui->lwTaskBatches->selectedItems();
			if (selectedItems.size() > 0)
			{
				for (auto item : selectedItems)
				{
					auto itr = _taskBatchList.find(item->text().toStdString());
					if (itr != _taskBatchList.end())
					{
						itr->second->reconfigureTaskBatch();
						_taskBatchRunner.addTaskBatch(itr->second->getTaskBatch());
					}
				}

				_taskRunnerThread.run([this]()
				{
					emit onSessionBegin();
					runTaskBatches();
					emit onSessionEnd();
					_taskRunnerThread.detach();
				});
			}
			else
			{
				QMessageBox::warning(_ui->pbRun, "Error", "No Task batches were selected.", QMessageBox::StandardButton::Ok);
			}
		}
		else
		{
			QMessageBox::warning(_ui->pbRun, "Error", "A Task batch is already running.", QMessageBox::StandardButton::Ok);
		}
	}

	void RunTaskBatchDialog::on_pbStop_clicked()
	{
		_taskBatchRunner.stop();
	}


} // namespace Elpida
