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

#ifndef ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
#define ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>
#include "Views/ConfigurationViews/TaskConfigurationListItemViewBase.hpp"
#include "Views/ConfigurationViews/ConfigurationValueViewBase.hpp"

class QVBoxLayout;
class QListWidgetItem;

namespace Elpida
{
	class BenchmarkConfigurationModel;
	class EventSubscriptionBase;
	class ConfigurationViewsPool;

	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkConfigurationView : public QWidget
	{
	Q_OBJECT

	public:
		explicit BenchmarkConfigurationView(BenchmarkConfigurationModel& benchmarkConfigurationModel,
			ConfigurationViewsPool& configurationViewsPool);
		~BenchmarkConfigurationView() override;
	private:
		std::vector<std::unique_ptr<ConfigurationValueViewBase>> _rentedViews;
		std::unordered_map<void*, std::unique_ptr<TaskConfigurationListItemViewBase>> _rentedItems;
		BenchmarkConfigurationModel& _benchmarkConfigurationModel;
		ConfigurationViewsPool& _configurationViewsPool;
		EventSubscriptionBase* _dataChangedSubscription;
		Ui::BenchmarkConfigurationView* _ui;
		QVBoxLayout* _containerLayout;

	signals:
		void dataChanged();

	private slots:
		void dataChangedHandler();
		void returnAllViewsToPool();
		void returnAllTaskListItems();
		static void taskListItemClicked(QListWidgetItem* item);
	};

} // namespace Elpida

#endif // ELPIDA_BENCHMARKCONFIGURATIONVIEW_HPP
