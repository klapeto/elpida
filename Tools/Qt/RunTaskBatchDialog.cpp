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
#include <TaskBatches/QtTaskBatchWrapper.hpp>
#include <QMessageBox>
#include <QTreeWidgetItem>

namespace Elpida
{

	RunTaskBatchDialog::RunTaskBatchDialog(const Map<String, QtTaskBatchWrapper*>& taskBatchList, QWidget *parent)
			: QDialog(parent), _taskBatchList(taskBatchList), _ui(new Ui::RunTaskBatchDialog)
	{
		_ui->setupUi(this);
		onTaskBatchListModified();
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

	void RunTaskBatchDialog::on_pbRun_clicked()
	{
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
			_taskBatchRunner.executeTasks();
			auto& results = _taskBatchRunner.getLastExecutionResults();

			for (auto& batchResult : results)
			{
				auto batchItem = new QTreeWidgetItem(_ui->twResults);
				batchItem->setText(0, QString::fromStdString(batchResult.first));
				for (auto& taskResults : batchResult.second)
				{
					auto taskItem = new QTreeWidgetItem(batchItem);
					taskItem->setText(0, QString::fromStdString(taskResults.first));
					for (auto& taskResult : taskResults.second)
					{
						auto taskResultItem = new QTreeWidgetItem(taskItem);
						taskResultItem->setText(0, QString::fromStdString(taskResult.getRunResult().getResultDescription()));
						taskResultItem->setText(1, QString::fromStdString(taskResult.getPerSecond()));
					}
				}
			}
		}
		else
		{
			QMessageBox::warning(_ui->pbRun, "Error", "No Task batches were selected.", QMessageBox::StandardButton::Ok);
		}
	}

} // namespace Elpida

