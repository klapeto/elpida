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
		const CpuInfoModel& _cpuInfoModel;
		CustomBenchmarkController& _customBenchmarkController;
		MemoryBenchmarkController& _memoryBenchmarkController;
		FullBenchmarkController& _fullBenchmarkController;
		Ui::MainWindow* _ui;

		QString _nonSelected = "<b>No selected. All Cpus will be used.</b>";

		void OnTopologyModelChanged();

	 private slots:
		void on_actionExit_triggered();
		void on_actionClear_results_triggered();
		void on_actionAbout_triggered();
	};
}
#endif // MAINWINDOW_HPP
