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

#include "BenchmarkListView.hpp"
#include "ui_BenchmarkListView.h"

#include <QList>
#include <QListWidgetItem>

#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Core/Commands/GetBenchmarksToRunCommand.hpp"
#include "Core/Commands/SelectedBenchmarkChangedEvent.hpp"
#include "Core/Abstractions/Mediator.hpp"

namespace Elpida
{

	BenchmarkListView::BenchmarkListView(const ListModel<Benchmark*>& model, Mediator& mediator)
		: QWidget(), CollectionModelObserver<Benchmark*>(model), _ui(new Ui::BenchmarkListView), _mediator(mediator)
	{
		_ui->setupUi(this);

		for (const auto& itm: model.getItems())
		{
			addItem(itm.getValue());
		}
		QWidget::connect(_ui->lvTaskBatches,
			&QListWidget::itemSelectionChanged,
			this,
			&BenchmarkListView::onSelectionChanged);
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
