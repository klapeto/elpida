#include "TaskResultsWidget.hpp"
#include "ui_TaskResultsWidget.h"

#include <Elpida/TaskBatch.hpp>

#include <QTreeWidgetItem>

namespace Elpida
{

	TaskResultsWidget::TaskResultsWidget(const CollectionModel<TaskBatchRunResult>& model)
		:
		QWidget(), CollectionModelObserver<TaskBatchRunResult>(model),
		_ui(new Ui::TaskResultsWidget)
	{
		_ui->setupUi(this);
	}

	TaskResultsWidget::~TaskResultsWidget()
	{
		delete _ui;
	}

	void TaskResultsWidget::onItemAdded(const TaskBatchRunResult& item)
	{
		auto parent = new QTreeWidgetItem();
		parent->setText(0, QString::fromStdString(item.getTaskBatch().getName()));
		for (const auto& throughput: item.getTasksThroughputs())
		{
			auto child = new QTreeWidgetItem();
			child->setText(0, QString::fromStdString(throughput.first));
			child->setText(1, QString::fromStdString(throughput.second.getRatePerSecondString()));
			parent->addChild(child);
		}
		_createdItems.emplace(item.getId(), parent);
		_ui->twResultList->addTopLevelItem(parent);
	}

	void TaskResultsWidget::onItemRemoved(const TaskBatchRunResult& item)
	{
		auto itr = _createdItems.find(item.getId());
		if (itr != _createdItems.end())
		{
			_ui->twResultList->removeItemWidget(itr->second, 0);
			_createdItems.erase(itr);
		}
	}

	void TaskResultsWidget::onCollectionCleared()
	{
		_ui->twResultList->clear();
		_createdItems.clear();
	}

} // namespace Elpida
