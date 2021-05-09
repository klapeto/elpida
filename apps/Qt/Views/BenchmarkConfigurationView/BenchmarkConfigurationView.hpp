/**************************************************************************
 *   Elpida - Benchmark library
 *
 *   Copyright (C) 2020 Ioannis Panagiotopoulos
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

#ifndef ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
#define ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>
#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"
#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"
#include "Models/Abstractions/CollectionModelObserver.hpp"
#include "Models/SelectedBenchmarksModel.hpp"
#include "RentedConfigurationView.hpp"

class QVBoxLayout;

class QListWidgetItem;

namespace Elpida
{
	class EventSubscriptionBase;
	class ConfigurationViewsPool;
	class BenchmarkConfigurationsCollectionModel;
	class Benchmark;
	class BenchmarkConfiguration;

	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkConfigurationView : public QWidget, public CollectionModelObserver<SelectedBenchmarksModel::Pair>
	{
	Q_OBJECT

	public:
		explicit BenchmarkConfigurationView(
				BenchmarkConfigurationsCollectionModel& benchmarkConfigurationsCollectionModel,
				ConfigurationViewsPool& configurationViewsPool,
				const SelectedBenchmarksModel& selectionModel
		);

		~BenchmarkConfigurationView() override;

	private:
		std::unordered_map<std::string, RentedConfigurationView> _rentedViews;
		BenchmarkConfigurationsCollectionModel& _benchmarkConfigurationsCollectionModel;
		ConfigurationViewsPool& _configurationViewsPool;
		Ui::BenchmarkConfigurationView* _ui;
		QVBoxLayout* _containerLayout;

	private slots:

		static void taskListItemClicked(QListWidgetItem* item);

	protected:
		void onCollectionCleared() override;

		void onItemAdded(const SelectedBenchmarksModel::Pair& item) override;

		void onItemRemoved(const SelectedBenchmarksModel::Pair& item) override;
	};

} // namespace Elpida

#endif // ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
