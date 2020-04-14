#include "TaskBatchesListWidget.hpp"
#include "ui_TaskBatchesListWidget.h"

#include <QList>
#include <QListWidgetItem>

#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/TaskBatch.hpp>
#include <Elpida/Task.hpp>
#include <TaskBatches/QtTaskBatchWrapper.hpp>

#include "Models/TaskBatchesModel.hpp"

namespace Elpida
{

	TaskBatchesListWidget::TaskBatchesListWidget(const CollectionModel<QtTaskBatchWrapper*>& model)
		: QWidget(), _ui(new Ui::TaskBatchesListWidget), _model(model)
	{
		_ui->setupUi(this);
//		_subscriptions.push_back(&_model.itemAdded.subscribe([this](const auto& item)
//		{
//			onItemAdded(item);
//		}));
//		_subscriptions.push_back(&_model.itemRemoved.subscribe([this](const auto& item)
//		{
//			onItemRemoved(item);
//		}));
//		_subscriptions.push_back(&_model.cleared.subscribe([this]()
//		{
//			onCleared();
//		}));
	}

	TaskBatchesListWidget::~TaskBatchesListWidget()
	{
		for (auto subscription: _subscriptions)
		{
			subscription->unsubscribe();
		}
		delete _ui;
	}

	QtTaskBatchWrapper* TaskBatchesListWidget::getSelectedTaskBatch()
	{
		auto selectedIndexes = _ui->lvTaskBatches->selectedItems();
		if (!selectedIndexes.empty())
		{
			auto variant = selectedIndexes.first()->data(0);
			// Dirty hack because QtTaskBatchWrapper is derived from QWidget and QVariant special handles that
			// which we do not want here.
			return (QtTaskBatchWrapper*)variant.value<void*>();
		}
		return nullptr;
	}

	void TaskBatchesListWidget::onItemAdded(const CollectionChangedEventArgs<QtTaskBatchWrapper*>& item)
	{
		auto value = item.getItem().getValue();
		auto wItem = new QListWidgetItem(QString::fromStdString(value->getTaskBatch().getName()));
		wItem->setData(0, QVariant::fromValue((void*)wItem));
		_ui->lvTaskBatches->addItem(wItem);
		_createdItems.insert_or_assign(value, wItem);
	}

	void TaskBatchesListWidget::onItemRemoved(const CollectionChangedEventArgs<QtTaskBatchWrapper*>& item)
	{
		auto itr = _createdItems.find(item.getItem().getValue());
		if (itr != _createdItems.end())
		{
			auto wItem = itr->second;
			_ui->lvTaskBatches->removeItemWidget(wItem);
			_createdItems.erase(itr);
		}
	}

	void TaskBatchesListWidget::onCleared()
	{
		_ui->lvTaskBatches->clear();
	}

} // namespace Elpida
