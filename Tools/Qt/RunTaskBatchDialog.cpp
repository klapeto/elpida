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
		_ui->setupUi(this);
		onTaskBatchListModified();
		_taskBatchRunner.taskStart.subscribe([this](const Runner::EventArguments::TaskStart& args)
		{	onTaskStart(args);});
		_taskBatchRunner.batchStart.subscribe([this](const Runner::EventArguments::BatchStart& args)
		{	onTaskBatchStart(args);});
		_taskBatchRunner.batchEnd.subscribe([this](const Runner::EventArguments::BatchEnd& args)
		{	onTaskBatchEnd(args);});
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

	void RunTaskBatchDialog::runTaskBatchesAndAppendResults()
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
		appendResults();
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
					runTaskBatchesAndAppendResults();
					_ui->pbStop->setEnabled(false);
					_ui->lblSatusValue->setText(_readyText);
					_ui->taskBatchRunProgress->setValue(0);
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

	void RunTaskBatchDialog::onTaskStart(const Runner::EventArguments::TaskStart& args)
	{
		_ui->lblCurrentTaskName->setText(QString::fromStdString(args.name));
		_ui->taskBatchRunProgress->setValue(_ui->taskBatchRunProgress->value() + 1);
	}

	void RunTaskBatchDialog::onTaskBatchStart(const Runner::EventArguments::BatchStart& args)
	{
		_ui->lblCurrentTaskBatchName->setText(QString::fromStdString(args.name));
		_ui->lblCurrentTaskName->setText(QString("N/A"));
		_ui->taskBatchRunProgress->setValue(0);
		_ui->taskBatchRunProgress->setMaximum(args.numberOfTasks);
	}

	void RunTaskBatchDialog::onTaskBatchEnd(const Runner::EventArguments::BatchEnd& args)
	{
		auto str = QString("N/A");
		_ui->lblCurrentTaskBatchName->setText(str);
		_ui->lblCurrentTaskName->setText(str);
	}

	void RunTaskBatchDialog::on_pbStop_clicked()
	{
		_taskBatchRunner.stop();
	}


} // namespace Elpida
