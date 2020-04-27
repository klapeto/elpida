#include "BenchmarkListView.hpp"
#include "ui_BenchmarkListView.h"

#include <QList>
#include <QListWidgetItem>

#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

#include "Models/BenchmarksModel.hpp"
#include "Core/Commands/GetBenchmarksToRunCommand.hpp"
#include "Core/Commands/SelectedBenchmarkChangedEvent.hpp"
#include "Core/Abstractions/Mediator.hpp"

namespace Elpida
{

	BenchmarkListView::BenchmarkListView(const ListModel<Benchmark*>& model, Mediator& mediator)
		: QWidget(), CollectionModelObserver<Benchmark*>(model), _mediator(mediator), _ui(new Ui::BenchmarkListView)
	{
		_ui->setupUi(this);

		for (const auto& itm: model.getItems())
		{
			addItem(itm.getValue());
		}
		QWidget::connect(_ui->lvTaskBatches, &QListWidget::itemSelectionChanged, this, &BenchmarkListView::onSelectionChanged);
	}

	BenchmarkListView::~BenchmarkListView()
	{
		delete _ui;
	}

	Benchmark* BenchmarkListView::getSelectedBenchmark()
	{
		auto selectedIndexes = _ui->lvTaskBatches->selectedItems();
		if (!selectedIndexes.empty())
		{
			auto variant = selectedIndexes.first()->data(Qt::UserRole);
			// Dirty hack because QtTaskBatchWrapper is derived from QWidget and QVariant special handles that
			// which we do not want here.
			return (Benchmark*)variant.value<void*>();
		}
		return nullptr;
	}

	void BenchmarkListView::onItemAdded(Benchmark* const& item)
	{
		addItem(item);
	}

	void BenchmarkListView::addItem(Benchmark* const& item)
	{
		auto wItem = new QListWidgetItem(QString::fromStdString(item->getName()));

		wItem->setData(Qt::UserRole, QVariant::fromValue((void*)item));
		_ui->lvTaskBatches->addItem(wItem);
		_createdItems.insert_or_assign(item, wItem);
	}

	void BenchmarkListView::onItemRemoved(Benchmark* const& item)
	{
		auto itr = _createdItems.find(item);
		if (itr != _createdItems.end())
		{
			auto wItem = itr->second;
			_ui->lvTaskBatches->removeItemWidget(wItem);
			_createdItems.erase(itr);
		}
	}

	void BenchmarkListView::onCollectionCleared()
	{
		_ui->lvTaskBatches->clear();
	}

	void BenchmarkListView::handle(GetBenchmarksToRunCommand& command)
	{
		auto selected = getSelectedBenchmark();
		if (selected != nullptr)
		{
			command.addBenchmark(*selected);
		}
	}

	void BenchmarkListView::onSelectionChanged()
	{
		SelectedBenchmarkChangedEvent event(*getSelectedBenchmark(), nullptr);
		_mediator.execute(event);
	}

} // namespace Elpida
