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

#include "ui_TaskBatchesWidget.h"
#include "TaskBatchesWidget.hpp"
#include "TaskBatchProperties.hpp"
#include "ListItemWithButton.hpp"
#include <Elpida/TaskBatch.hpp>
#include <Elpida/Types/String.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include <QMessageBox>
#include <QListWidgetItem>
#include <QtCharts/QChartView>

namespace Elpida
{

	TaskBatchesWidget::TaskBatchesWidget(const Map<String, QtTaskBatchWrapper*>& taskBatchList, QWidget *parent)
			: QWidget(parent), _taskBatchList(taskBatchList), _ui(new Ui::TaskBatchesWidget)
	{
		_runningText = "<span style=\" color:#b50000;\">Running</span>";
		_readyText = "<span style=\"color:#008d09;\">Ready</span>";
		_na = "N/A";
		_ui->setupUi(this);
		_taskBatchPropertiesDialog = new TaskBatchProperties(this);
		_initialChart = _ui->wChart->chart();
		_ui->wChart->setRenderHint(QPainter::Antialiasing);
		_initialChart->setTitle("No Data");
		onTaskBatchListModified();
		_ui->splitter_2->setStretchFactor(0, 1);

		QPushButton::connect(this, &TaskBatchesWidget::onTaskBatchStart, this, &TaskBatchesWidget::updateForTaskBatchBegin);
		QPushButton::connect(this, &TaskBatchesWidget::onTaskStart, this, &TaskBatchesWidget::updateForTaskBegin);
		QPushButton::connect(this, &TaskBatchesWidget::onTaskEnd, this, &TaskBatchesWidget::updateForTaskEnd);
		QPushButton::connect(this, &TaskBatchesWidget::onTaskBatchEnd, this, &TaskBatchesWidget::updateForTaskBatchEnd);
		QPushButton::connect(this, &TaskBatchesWidget::onSessionBegin, this, &TaskBatchesWidget::updateForSessionBegin);
		QPushButton::connect(this, &TaskBatchesWidget::onSessionEnd, this, &TaskBatchesWidget::updateForSessionEnd);

		_taskBatchRunner.taskStart.subscribe([this](const Runner::EventArguments::TaskStart& args)
		{
			emit onTaskStart(QString::fromStdString(args.name));
		});

		_taskBatchRunner.taskEnd.subscribe([this](const Runner::EventArguments::TaskEnd& args)
		{
			emit onTaskEnd(QString::fromStdString(args.name));
		});

		_taskBatchRunner.batchStart.subscribe([this](const Runner::EventArguments::BatchStart& args)
		{
			emit onTaskBatchStart(QString::fromStdString(args.name), args.numberOfTasks);
		});

		_taskBatchRunner.batchEnd.subscribe([this](const Runner::EventArguments::BatchEnd& args)
		{
			emit onTaskBatchEnd(QString::fromStdString(args.name));
		});
	}

	TaskBatchesWidget::~TaskBatchesWidget()
	{
		_ui->wChart->setChart(_initialChart);
		delete _ui;
	}

	void TaskBatchesWidget::onTaskBatchListModified()
	{
		int childrenSize = _ui->lwTaskBatches->count(), c = 0;
		for (auto& taskBatch : _taskBatchList)
		{
			auto item = static_cast<QListWidgetItem*>(_ui->lwTaskBatches->item(c++));
			if (item != nullptr)
			{
				auto wrapper = static_cast<ListItemWithButton*>(_ui->lwTaskBatches->itemWidget(item));
				if (wrapper != nullptr)
				{
					wrapper->setText(QString::fromStdString(taskBatch.second->getTaskBatch().getName()));
				}
				else
				{
					item->setText(QString::fromStdString(taskBatch.second->getTaskBatch().getName()));
				}
			}
			else
			{
				item = new QListWidgetItem(_ui->lwTaskBatches);
				auto wrapper = new ListItemWithButton(QString::fromStdString(taskBatch.second->getTaskBatch().getName()));
				_ui->lwTaskBatches->setItemWidget(item, wrapper);
				item->setSizeHint(wrapper->sizeHint());
				QWidget::connect(wrapper, &ListItemWithButton::buttonClicked, this, &TaskBatchesWidget::onListItemButtonClicked);
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

	void TaskBatchesWidget::appendResults()
	{
		auto& results = _taskBatchRunner.getLastExecutionResults();
		for (auto& batchResult : results)
		{
			auto batchItem = new QTreeWidgetItem(_ui->twResults);
			batchItem->setText(0, QString::fromStdString(batchResult.first));
			_cachedResults.emplace(batchItem, batchResult.second);
			for (auto& taskResults : batchResult.second)
			{
				auto taskItem = new QTreeWidgetItem(batchItem);
				taskItem->setText(0, QString::fromStdString(taskResults.first));
				if (taskResults.second.size() == 1)
				{
					taskItem->setText(1, QString::fromStdString(taskResults.second[0].getRatePerSecondString()));
				}
				else
				{
					for (auto& taskResult : taskResults.second)
					{
						auto taskResultItem = new QTreeWidgetItem(taskItem);
						taskResultItem->setText(0, QString::fromStdString(taskResult.getRunResult().getResultDescription()));
						taskResultItem->setText(1, QString::fromStdString(taskResult.getRatePerSecondString()));
					}
				}
			}
		}
	}

	void TaskBatchesWidget::updateForTaskBatchBegin(const QString& name, int size)
	{
		_ui->taskBatchRunProgress->setMaximum(size);
		_ui->lblCurrentTaskBatchName->setText(name);
	}

	void TaskBatchesWidget::updateForTaskBegin(const QString& name)
	{
		_ui->lblCurrentTaskName->setText(name);
	}

	void TaskBatchesWidget::updateForTaskEnd(const QString& name)
	{
		_ui->taskBatchRunProgress->setValue(_ui->taskBatchRunProgress->value() + 1);
	}

	void TaskBatchesWidget::updateForTaskBatchEnd(const QString& name)
	{
		_ui->taskBatchRunProgress->setValue(0);
	}

	void TaskBatchesWidget::updateForSessionBegin()
	{
		_ui->lblSatusValue->setText(_runningText);
		_ui->pbStop->setEnabled(true);
	}

	void TaskBatchesWidget::updateForSessionEnd()
	{
		_ui->lblSatusValue->setText(_readyText);
		_ui->lblCurrentTaskName->setText(_na);
		_ui->lblCurrentTaskBatchName->setText(_na);
		_ui->pbStop->setEnabled(false);
		appendResults();
	}

	void TaskBatchesWidget::runTaskBatches()
	{
		try
		{
			_taskBatchRunner.executeTasks();
		}
		catch (ElpidaException& e)
		{
			QMessageBox::critical(this, "Error", QString::fromStdString("Task batch runner produced error: " + e.getMessage()),
			                      QMessageBox::StandardButton::Ok);
		}
	}

	void TaskBatchesWidget::on_pbRun_clicked()
	{
		if (!_taskRunnerThread.isRunning())
		{
			_taskBatchRunner.clearTaskBatches();
			auto selectedItems = _ui->lwTaskBatches->selectedItems();
			if (selectedItems.size() > 0)
			{
				_ui->lblSatusValue->setText(_runningText);
				_ui->pbStop->setEnabled(true);
				for (auto item : selectedItems)
				{
					std::string name;
					auto wrapper = static_cast<ListItemWithButton*>(_ui->lwTaskBatches->itemWidget(item));
					if (wrapper != nullptr)
					{
						name = wrapper->getText().toStdString();
					}
					else
					{
						name = item->text().toStdString();
					}
					auto itr = _taskBatchList.find(name);
					if (itr != _taskBatchList.end())
					{
						try
						{
							itr->second->reconfigureTaskBatch();
							itr->second->validateConfiguration();
						}
						catch (const Elpida::ElpidaException& e)
						{
							QMessageBox::critical(
							        this,
							        "Error",
							        QString::fromStdString(
							                "A task batch was not configured properly: '" + itr->second->getTaskBatch().getName()
							                        + "'. Error: " + e.getMessage()),
							        QMessageBox::StandardButton::Ok);
							continue;
						}
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
				QMessageBox::warning(this, "Error", "No Task batches were selected.", QMessageBox::StandardButton::Ok);
			}
		}
		else
		{
			QMessageBox::warning(this, "Error", "A Task batch is already running.", QMessageBox::StandardButton::Ok);
		}
	}

	void TaskBatchesWidget::on_pbStop_clicked()
	{
		_taskBatchRunner.stop();
	}

	void TaskBatchesWidget::onListItemButtonClicked(const QString& name)
	{
		auto pageItr = _taskBatchList.find(name.toStdString());
		if (pageItr != _taskBatchList.end())
		{
			if (pageItr->second != nullptr && pageItr->second->hasProperties())
			{
				_taskBatchPropertiesDialog->setPage(pageItr->second);
				_taskBatchPropertiesDialog->show();
			}
			else
			{
				QMessageBox::information(this, "Task Batch Properties", "This task batch does not export properties",
				                         QMessageBox::StandardButton::Ok);
			}
		}
	}

	void TaskBatchesWidget::on_twResults_itemClicked(QTreeWidgetItem *item, int column)
	{
		auto pageItr = _taskBatchList.find(item->text(0).toStdString());
		if (pageItr != _taskBatchList.end())
		{
			if (pageItr->second->hasResultChart())
			{
				auto resultItr = _cachedResults.find(item);
				if (resultItr != _cachedResults.end())
				{
					pageItr->second->updateResultsChartData(resultItr->second);
					_ui->wChart->setChart(pageItr->second->getResultsChartContainer());
				}
			}
			else
			{
				_ui->wChart->setChart(_initialChart);
			}
		}
		else
		{
			_ui->wChart->setChart(_initialChart);
		}
	}

} // namespace Elpida

