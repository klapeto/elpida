#ifndef MEMORYOVERHEADVIEW_HPP
#define MEMORYOVERHEADVIEW_HPP

#include <QWidget>
#include <QtCharts>
#include "EventSubscription.hpp"
#include "Models/MemoryBenchmark/MemoryBenchmarkResultModel.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Core/ThreadQueue.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class MemoryBenchmarkView;
	}

	class BenchmarkRunConfigurationModel;
	class BenchmarkRunConfigurationController;
	class BenchmarkRunConfigurationView;
	class MemoryBenchmarkController;
	class MemoryBenchmarkModel;

	class MemoryBenchmarkView : public QWidget
	{
	Q_OBJECT

	public:
		explicit MemoryBenchmarkView(
				const MemoryBenchmarkModel& model,
				MemoryBenchmarkController& controller,
				const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
				BenchmarkRunConfigurationController& benchmarkRunConfigurationController, QWidget* parent = nullptr);
		~MemoryBenchmarkView() override;

	private:
		Ui::MemoryBenchmarkView* _ui;
		BenchmarkRunConfigurationView* _runConfigurationView;
		MemoryBenchmarkController& _controller;
		const MemoryBenchmarkModel& _model;

		EventSubscription<bool> _runningChanged;
		EventSubscription<const CollectionItem<MemoryBenchmarkResultModel>&> _itemAdded;
		EventSubscription<const std::string&> _currentBenchmarkChanged;
		EventSubscription<> _resultsCleared;
		std::weak_ptr<ThreadQueue> _threadQueue;

		QLineSeries* _series;
		QChart* _chart;
		bool _updatingUi;

		int _currentBenchmarkIndex;
		int _maxBenchmarkIndex;
		Score _maxScore;
		Score _minScore;
		double _minWorkingSet;
		double _maxWorkingSet;

		void UpdateProgress();
		void OnResultAdded(const MemoryBenchmarkResultModel& result);
		void OnResultsCleared();
		void SetMemoryLatencyUi();
		void SetMemoryBandwidthUi();

		void OnMemoryLatencyResultAdded(const MemoryBenchmarkResultModel& result, QString& stringScore, Score& scoreToShow);
		void OnMemoryBandwidthResultAdded(const MemoryBenchmarkResultModel& result, QString& stringScore, Score& scoreToShow);
		void ResetScores();

	private slots:
		void on_bpStart_clicked(bool checked);
		void on_spnMultiplier_valueChanged(double value);
		void on_spnStartSize_valueChanged(int value);
		void on_spnIterations_valueChanged(int value);
		void on_rbMemoryReadBandwidth_toggled(bool value);
		void on_rbMemoryLatency_toggled(bool value);

		void OnBenchmarkStopped();
		void OnBenchmarkStarted();
	};
}

#endif // MEMORYOVERHEADVIEW_HPP
