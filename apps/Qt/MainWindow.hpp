//  Copyright (c) 2025  Ioannis Panagiotopoulos
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

#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>
#include <QLabel>

#include "EventSubscription.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class MainWindow;
	}

	class OsInfoModel;
	class MemoryInfoModel;
	class CpuInfoModel;
	class TimingModel;
	class TopologyModel;
	class BenchmarkRunConfigurationModel;
	class BenchmarkRunConfigurationController;
	class CustomBenchmarkModel;
	class CustomBenchmarkController;
	class FullBenchmarkModel;
	class FullBenchmarkController;
	class ConfigurationViewPool;
	class MemoryBenchmarkModel;
	class MemoryBenchmarkController;
	class DesktopService;

	class MainWindow : public QMainWindow
	{
	 Q_OBJECT
	 public:
		explicit MainWindow(
			const OsInfoModel& osInfoModel,
			const MemoryInfoModel& memoryInfoModel,
			const CpuInfoModel& cpuInfoModel,
			const TimingModel& timingModel,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			const FullBenchmarkModel& fullBenchmarkModel,
			const CustomBenchmarkModel& customBenchmarksModel,
			const MemoryBenchmarkModel& memoryOverheadCalculationModel,
			TopologyModel& topologyModel,
			const DesktopService& desktopService,
			FullBenchmarkController& fullBenchmarkController,
			CustomBenchmarkController& customBenchmarksController,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
			MemoryBenchmarkController& memoryOverheadCalculationController,
			ConfigurationViewPool& configurationViewPool,
			QWidget* parent = nullptr);

		~MainWindow() override;
	 private:
		EventSubscription<> _topologyModelChanged;
		TopologyModel& _topologyModel;
		QLabel* _selectedNodesLabel;
		CustomBenchmarkController& _customBenchmarkController;
		MemoryBenchmarkController& _memoryBenchmarkController;
		FullBenchmarkController& _fullBenchmarkController;
		const DesktopService& _desktopService;
		Ui::MainWindow* _ui;

		QString _nonSelected = "<b>No selected. All Cpus will be used.</b>";

		void OnTopologyModelChanged();

	 private slots:
		void on_actionExit_triggered();
		void on_actionClear_results_triggered();
		void on_actionAbout_triggered();
		void on_actionDocumentation_triggered();
		void on_actionSave_results_as_triggered();
	};
}
#endif // MAINWINDOW_HPP
