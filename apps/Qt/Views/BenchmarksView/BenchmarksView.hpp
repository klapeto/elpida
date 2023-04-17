#ifndef BENCHMARKSVIEW_HPP
#define BENCHMARKSVIEW_HPP

#include <QWidget>
#include <QTreeWidgetItem>
#include <unordered_map>

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarksView;
	}

	class BenchmarksModel;
	class BenchmarkModel;
	class BenchmarkConfigurationView;
	class BenchmarkConfigurationModel;
	class BenchmarkResultsView;
	class BenchmarksController;
	class ConfigurationViewPool;

	class BenchmarksView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit BenchmarksView(
			const BenchmarksModel& benchmarksModel,
			const BenchmarkConfigurationModel& benchmarkConfigurationModel,
			BenchmarksController& benchmarksController,
			ConfigurationViewPool& configurationViewPool);
		~BenchmarksView() override;
	 private slots:
		void on_twBenchmarks_currentItemChanged(QTreeWidgetItem* current, QTreeWidgetItem* previous);
		void on_chkUpload_stateChanged(int state);
		void on_chkOpenResult_stateChanged(int state);
		void on_spnTimes_valueChanged(int value);
	 private:
		Ui::BenchmarksView* _ui;
		const BenchmarksModel& _benchmarksModel;
		BenchmarksController& _benchmarksController;
		BenchmarkConfigurationView* _configurationView;
		BenchmarkResultsView* _resultsView;
		std::unordered_map<QString, BenchmarkModel*> _benchmarkMap;
		bool _uiUpdating;
		void UpdateUi();
		void LoadBenchmarkTree();
	};
}

#endif // BENCHMARKSVIEW_HPP
