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

#ifndef APPS_QT_UI_TASKBATCHESLISTWIDGET_BENCHMARKLISTVIEW_HPP
#define APPS_QT_UI_TASKBATCHESLISTWIDGET_BENCHMARKLISTVIEW_HPP

#include <QWidget>
#include "Models/Abstractions/CollectionModelObserver.hpp"
#include "Models/Benchmarks/BenchmarkGroup.hpp"
#include "Core/Abstractions/CommandHandler.hpp"
#include <Elpida/Utilities/OptionalReference.hpp>

#include <unordered_map>

class QTreeWidgetItem;

namespace Elpida
{
	class Mediator;

	class SelectedBenchmarksModel;
	class BenchmarksModel;

	namespace Ui
	{
		class BenchmarkListView;
	}

	class BenchmarkListView :
			public QWidget,
			public CollectionModelObserver<BenchmarkGroup>
	{
	Q_OBJECT

	public:

		explicit BenchmarkListView(const BenchmarksModel& benchmarksModel,
				SelectedBenchmarksModel& selectionModel,
				Mediator& mediator);
		~BenchmarkListView() override;
	private:
		Ui::BenchmarkListView* _ui;
		std::unordered_map<const BenchmarkGroup*, QTreeWidgetItem*> _createdItems;
		Mediator& _mediator;
		SelectedBenchmarksModel& _selectionModel;
	protected:

		void onItemAdded(const BenchmarkGroup& item) override;
		void onItemRemoved(const BenchmarkGroup& item) override;
		void onCollectionCleared() override;
		void addItem(const BenchmarkGroup& item);

	private slots:
		void onSelectionChanged();
	};

} // namespace Elpida

#endif // APPS_QT_UI_TASKBATCHESLISTWIDGET_BENCHMARKLISTVIEW_HPP
