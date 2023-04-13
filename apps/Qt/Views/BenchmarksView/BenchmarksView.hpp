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
	class BenchmarksView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit BenchmarksView(BenchmarksModel& benchmarksModel);
		~BenchmarksView() override;
	 private slots:
		void on_twBenchmarks_currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous);
	 private:
		Ui::BenchmarksView* _ui;
		BenchmarksModel& _benchmarksModel;
		BenchmarkConfigurationView* _configurationView;
		std::unordered_map<QString, BenchmarkModel*> _benchmarkMap;
	};
}

#endif // BENCHMARKSVIEW_HPP
