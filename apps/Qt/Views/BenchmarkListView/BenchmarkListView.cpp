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
#include <unordered_set>

#include <Elpida/SharedLibraryLoader.hpp>
#include <Elpida/Engine/Benchmark/Benchmark.hpp>

#include "Models/Abstractions/ListModel/ListModel.hpp"
#include "Models/Benchmarks/BenchmarksModel.hpp"
#include "Models/SelectedBenchmarksModel.hpp"
#include "Core/Commands/GetBenchmarksToRunCommand.hpp"
#include "Core/Commands/SelectedBenchmarkChangedEvent.hpp"
#include "Core/Abstractions/Mediator.hpp"

namespace Elpida
{

	BenchmarkListView::BenchmarkListView(const BenchmarksModel& benchmarksModel,
			SelectedBenchmarksModel& selectionModel,
			Mediator& mediator)
			: QWidget(), CollectionModelObserver<BenchmarkGroup>(benchmarksModel),
			  _ui(new Ui::BenchmarkListView),
			  _mediator(mediator),
			  _selectionModel(selectionModel)
	{
		_ui->setupUi(this);

		for (const auto& itm: benchmarksModel.getItems())
		{
			addItem(itm.getValue());
		}
		QWidget::connect(_ui->lvBenchmarkGroups,
				&QTreeWidget::itemSelectionChanged,
				this,
				&BenchmarkListView::onSelectionChanged);
	}

	BenchmarkListView::~BenchmarkListView()
	{
		delete _ui;
	}

	void BenchmarkListView::onItemAdded(const BenchmarkGroup& item)
	{
		addItem(item);
	}

	void BenchmarkListView::addItem(const BenchmarkGroup& item)
	{
		auto wItem = new QTreeWidgetItem();
		wItem->setText(0, QString::fromStdString(item.getName()));

		for (auto& benchmark: item.getBenchmarks())
		{
			auto treeItem = new QTreeWidgetItem();
			treeItem->setText(0, QString::fromStdString(benchmark->getName()));
			treeItem->setData(0, Qt::UserRole, QVariant::fromValue((void*)benchmark.get()));
			wItem->addChild(treeItem);
		}

		_ui->lvBenchmarkGroups->addTopLevelItem(wItem);
		_createdItems.insert_or_assign(&item, wItem);
	}

	void BenchmarkListView::onItemRemoved(const BenchmarkGroup& item)
	{
		auto itr = _createdItems.find(&item);
		if (itr != _createdItems.end())
		{
			auto wItem = itr->second;
			_ui->lvBenchmarkGroups->removeItemWidget(wItem, 0);
			_createdItems.erase(itr);
		}
	}

	void BenchmarkListView::onCollectionCleared()
	{
		_ui->lvBenchmarkGroups->clear();
	}

	void BenchmarkListView::onSelectionChanged()
	{
		auto selectedIndexes = _ui->lvBenchmarkGroups->selectedItems();
		if (!selectedIndexes.empty())
		{
			_selectionModel.transactional<SelectedBenchmarksModel>(
					[&selectedIndexes](SelectedBenchmarksModel& list)
					{
						std::unordered_set<std::string> notUsedKeys;
						for (auto& [key, value] : list)
						{
							notUsedKeys.emplace(key);
						}

						for (auto& index: selectedIndexes)
						{
							auto variant = index->data(0, Qt::UserRole);
							auto ptr = ((Benchmark*)variant.value<void*>());

							if (ptr != nullptr)
							{
								auto key = ptr->getUuid();
								notUsedKeys.erase(key);
								if (!list.containsKey(key))
								{
									list.add(ptr->getUuid(), *ptr);
								}
							}
						}

						for (auto& key: notUsedKeys)
						{
							list.remove(key);
						}
					});
		}
		else
		{
			if (!_selectionModel.empty())
			{
				_selectionModel.clear();
			}
		}
	}

} // namespace Elpida
