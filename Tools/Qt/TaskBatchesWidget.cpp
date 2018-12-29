#include "ui_TaskBatchesWidget.h"
#include "TaskBatchesWidget.hpp"
#include "TaskBatchProperties.hpp"
#include <Elpida/TaskBatch.hpp>
#include <Elpida/Types/String.hpp>
#include <Elpida/Exceptions/ElpidaException.hpp>
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include <QMessageBox>
#include <QListWidgetItem>

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

		onTaskBatchListModified();

		QPushButton::connect(this, &TaskBatchesWidget::onTaskBatchStart, this, &TaskBatchesWidget::updateForTaskBatchBegin);
		QPushButton::connect(this, &TaskBatchesWidget::onTaskStart, this, &TaskBatchesWidget::updateForTaskBegin);
		QPushButton::connect(this, &TaskBatchesWidget::onTaskBatchEnd, this, &TaskBatchesWidget::updateForTaskBatchEnd);
		QPushButton::connect(this, &TaskBatchesWidget::onSessionBegin, this, &TaskBatchesWidget::updateForSessionBegin);
		QPushButton::connect(this, &TaskBatchesWidget::onSessionEnd, this, &TaskBatchesWidget::updateForSessionEnd);

		_taskBatchRunner.taskStart.subscribe([this](const Runner::EventArguments::TaskStart& args)
		{	emit onTaskStart(QString::fromStdString(args.name));});
		_taskBatchRunner.batchStart.subscribe([this](const Runner::EventArguments::BatchStart& args)
		{	emit onTaskBatchStart(QString::fromStdString(args.name), args.numberOfTasks);});
		_taskBatchRunner.batchEnd.subscribe([this](const Runner::EventArguments::BatchEnd& args)
		{	emit onTaskBatchEnd(QString::fromStdString(args.name));});
	}

	TaskBatchesWidget::~TaskBatchesWidget()
	{
		delete _ui;
	}

	void TaskBatchesWidget::onTaskBatchListModified()
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

	void TaskBatchesWidget::appendResults()
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

	void TaskBatchesWidget::updateForTaskBatchBegin(const QString& name, int size)
	{
		_ui->taskBatchRunProgress->setMaximum(size);
		_ui->lblCurrentTaskBatchName->setText(name);
	}

	void TaskBatchesWidget::updateForTaskBegin(const QString& name)
	{
		_ui->taskBatchRunProgress->setValue(_ui->taskBatchRunProgress->value() + 1);
		_ui->lblCurrentTaskName->setText(name);
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
							        QString::fromStdString("A task batch was not configured properly: '" + itr->second->getTaskBatch().getName() + "'. Error: "
							                + e.getMessage()),
							        QMessageBox::StandardButton::Ok);
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

	void TaskBatchesWidget::on_lwTaskBatches_itemDoubleClicked(QListWidgetItem *item)
	{
		auto taskBatchName = item->text().toStdString();
		auto pageItr = _taskBatchList.find(taskBatchName);
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

} // namespace Elpida

