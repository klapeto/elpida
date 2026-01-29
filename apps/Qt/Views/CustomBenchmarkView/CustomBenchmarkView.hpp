/*
 *  Copyright (c) 2025  Ioannis Panagiotopoulos
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */

#ifndef BENCHMARKSVIEW_HPP
#define BENCHMARKSVIEW_HPP

#include <QWidget>
#include <QTreeWidgetItem>
#include <unordered_map>
#include <string>

namespace Elpida::Application
{
	namespace Ui
	{
		class CustomBenchmarkView;
	}


	class BenchmarkRunConfigurationView;
	class BenchmarkRunConfigurationController;
	class BenchmarkRunConfigurationModel;
	class BenchmarkModel;
	class CustomBenchmarkModel;
	class CustomBenchmarkConfigurationView;
	class CustomBenchmarkResultsView;
	class CustomBenchmarkController;
	class ConfigurationViewPool;

	class CustomBenchmarkView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit CustomBenchmarkView(
			const CustomBenchmarkModel& benchmarksModel,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			CustomBenchmarkController& benchmarksController,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
			ConfigurationViewPool& configurationViewPool);
		~CustomBenchmarkView() override;
	 private slots:
		void on_twBenchmarks_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
		void on_pbRun_clicked(bool checked);
	 private:
		Ui::CustomBenchmarkView* _ui;
		const CustomBenchmarkModel& _customBenchmarksModel;
		CustomBenchmarkController& _customBenchmarksController;
		CustomBenchmarkConfigurationView* _configurationView;
		BenchmarkRunConfigurationView* _runConfigurationView;
		CustomBenchmarkResultsView* _resultsView;
		std::unordered_map<std::string, BenchmarkModel*> _benchmarkMap;
		bool _uiUpdating;

		void Run();
		void UpdateUi();
		void LoadBenchmarkTree();
	};
}

#endif // BENCHMARKSVIEW_HPP
