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

#ifndef ELPIDA_TASKRESULTSWIDGET_HPP
#define ELPIDA_TASKRESULTSWIDGET_HPP

#include <QWidget>
#include <Elpida/Engine/Result/BenchmarkResult.hpp>
#include "Models/Abstractions/CollectionModelObserver.hpp"

class QTreeWidgetItem;

namespace Elpida
{

	namespace Ui
	{
		class BenchmarkResultsView;
	}

	class BenchmarkResultsView : public QWidget, public CollectionModelObserver<BenchmarkResult>
	{
	Q_OBJECT

	public:
		explicit BenchmarkResultsView(const CollectionModel<BenchmarkResult>& model);
		~BenchmarkResultsView() override;
	private:
		Ui::BenchmarkResultsView* _ui;
		std::unordered_map<std::string, QTreeWidgetItem*> _createdItems;
	protected:
		void onItemAdded(const BenchmarkResult& item) override;
		void onItemRemoved(const BenchmarkResult& item) override;
		void onCollectionCleared() override;
	};

} // namespace Elpida

#endif // ELPIDA_TASKRESULTSWIDGET_HPP
