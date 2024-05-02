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
	class CustomBenchmarkResultsModel;
	class CustomBenchmarkController;
	class FullBenchmarkModel;
	class FullBenchmarkController;
	class ConfigurationViewPool;
	class MainController;

	class MainWindow : public QMainWindow
	{
	 Q_OBJECT
	 public:
		explicit MainWindow(
				MainController& mainController,
			const OsInfoModel& osInfoModel,
			const MemoryInfoModel& memoryInfoModel,
			const CpuInfoModel& cpuInfoModel,
			const TimingModel& timingModel,
			const CustomBenchmarkResultsModel& customBenchmarkResultsModel,
			const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			const FullBenchmarkModel& fullBenchmarkModel,
			TopologyModel& topologyModel,
			CustomBenchmarkModel& customBenchmarksModel,
			CustomBenchmarkController& customBenchmarksController,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
			FullBenchmarkController& fullBenchmarkController,
			ConfigurationViewPool& configurationViewPool,
			QWidget* parent = nullptr);

		~MainWindow() override;
	 private:
		EventSubscription<> _topologyModelChanged;
		TopologyModel& _topologyModel;
		QLabel* _selectedNodesLabel;
		MainController& _mainController;
		const CpuInfoModel& _cpuInfoModel;
		Ui::MainWindow* _ui;

		QString _nonSelected = "<b>No selected. All Cpus will be used.</b>";

		void OnTopologyModelChanged();

	 private slots:
		void on_actionExit_triggered();
		void on_actionSave_results_as_triggered();
		void on_actionAbout_triggered();
	};
}
#endif // MAINWINDOW_HPP
