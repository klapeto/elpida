//
//  Copyright (c) 2024  Ioannis Panagiotopoulos
//
//  This program is free software: you can redistribute it and/or modify
//  it under the terms of the GNU General Public License as published by
//  the Free Software Foundation, either version 3 of the License, or
//  (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program.  If not, see <https://www.gnu.org/licenses/>.

#ifndef CUSTOMBENCHMARKCONFIGURATIONVIEW_HPP
#define CUSTOMBENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>
#include <vector>
#include <unordered_map>

#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Controllers/BenchmarkConfigurationInstanceController.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class CustomBenchmarkConfigurationView;
	}

	class CustomBenchmarkModel;
	class BenchmarkModel;
	class BenchmarkConfigurationModel;
	class ConfigurationView;
	class ConfigurationViewPool;

	class CustomBenchmarkConfigurationView : public QWidget
	{
	 Q_OBJECT
	 public:
		explicit CustomBenchmarkConfigurationView(const CustomBenchmarkModel& benchmarksModel, ConfigurationViewPool& configurationViewPool);
		~CustomBenchmarkConfigurationView() override;
	 private:
		Ui::CustomBenchmarkConfigurationView* _ui;
		const CustomBenchmarkModel& _benchmarksModel;
		ConfigurationViewPool& _configurationViewPool;
		const BenchmarkModel* _currentBenchmark;
		EventSubscription<> _currentBenchmarkChanged;
		std::unordered_map<const BenchmarkConfigurationModel*, ConfigurationView*> _rentedViews;

		void ClearViews();
		void Remove(const BenchmarkConfigurationModel& model);
		void Add(const BenchmarkConfigurationModel& model);
	};
}

#endif // CUSTOMBENCHMARKCONFIGURATIONVIEW_HPP
