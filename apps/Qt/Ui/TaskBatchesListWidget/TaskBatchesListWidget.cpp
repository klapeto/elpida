#include "TaskBatchesListWidget.hpp"
#include "ui_TaskBatchesListWidget.h"

#include <QList>
#include <QListWidgetItem>

#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/Engine/Benchmark.hpp>

#include "Models/TaskBatchesModel.hpp"
#include "Core/Commands/GetSelectedTaskBatchesCommand.hpp"

namespace Elpida
{

	TaskBatchesListWidget::TaskBatchesListWidget(const CollectionModel<Benchmark*>& model)
		: QWidget(), CollectionModelObserver<Benchmark*>(model), _ui(new Ui::TaskBatchesListWidget)
	{
		_ui->setupUi(this);

		for (const auto& itm: model.getItems())
		{
			addItem(itm.getValue());
		}
	}

	TaskBatchesListWidget::~TaskBatchesListWidget()
	{
		delete _ui;
	}

	TaskBatch* TaskBatchesListWidget::getSelectedTaskBatch()
	{
		auto selectedIndexes = _ui->lvTaskBatches->selectedItems();
		if (!selectedIndexes.empty())
		{
			auto variant = selectedIndexes.first()->data(Qt::UserRole);
			// Dirty hack because QtTaskBatchWrapper is derived from QWidget and QVariant special handles that
			// which we do not want here.
			return (TaskBatch*)variant.value<void*>();
		}
		return nullptr;
	}

	void TaskBatchesListWidget::onItemAdded(Benchmark* const& item)
	{
		addItem(item);
	}

	void TaskBatchesListWidget::addItem(Benchmark* const& item)
	{
		auto wItem = new QListWidgetItem(QString::fromStdString(item->getName()));
		wItem->setData(Qt::UserRole, QVariant::fromValue((void*)&item));
		_ui->lvTaskBatches->addItem(wItem);
		_createdItems.insert_or_assign(item, wItem);
	}

	void TaskBatchesListWidget::onItemRemoved(Benchmark* const& item)
	{
		auto itr = _createdItems.find(item);
		if (itr != _createdItems.end())
		{
			auto wItem = itr->second;
			_ui->lvTaskBatches->removeItemWidget(wItem);
			_createdItems.erase(itr);
		}
	}

	void TaskBatchesListWidget::onCollectionCleared()
	{
		_ui->lvTaskBatches->clear();
	}

	void TaskBatchesListWidget::handle(GetSelectedTaskBatchesCommand& command)
	{
		auto selected = getSelectedTaskBatch();
		if (selected != nullptr)
		{
			command.addTaskBatch(*selected);
		}
	}

} // namespace Elpida
