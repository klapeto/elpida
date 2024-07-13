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
