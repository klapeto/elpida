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
	class BenchmarksModel;
	class BenchmarkResultsModel;
	class BenchmarksController;
	class ConfigurationViewPool;

	class MainWindow : public QMainWindow
	{
	 Q_OBJECT
	 public:
		explicit MainWindow(
			const OsInfoModel& osInfo,
			const MemoryInfoModel& memoryInfo,
			const CpuInfoModel& cpuInfo,
			const TimingModel& overheadsInfo,
			TopologyModel& topologyModel,
			BenchmarksModel& benchmarksModel,
			const BenchmarkResultsModel& benchmarkResultsModel,
			BenchmarksController& benchmarksController,
			ConfigurationViewPool& configurationViewPool,
			QWidget* parent = nullptr);

		~MainWindow() override;
	 private:
		EventSubscription<> _topologyModelChanged;
		EventSubscription<> _benchmarksModelChanged;
		TopologyModel& _topologyModel;
		BenchmarksModel& _benchmarksModel;
		QLabel* _selectedNodesLabel;
		QLabel* _selectedBenchmarkLabel;
		QString _nonSelected = "<b style=\"color: #d73e3e;\">No selected</b>";
		Ui::MainWindow* _ui;

		void OnTopologyModelChanged();
		void OnBenchmarksModelChanged();

	 private slots:
		void on_actionExit_triggered();
		void on_actionSave_results_as_triggered();
		void on_actionAbout_triggered();
	};
}
#endif // MAINWINDOW_HPP
