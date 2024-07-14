#ifndef BENCHMARKVIEW_HPP
#define BENCHMARKVIEW_HPP

#include <QWidget>
#include <memory>
#include <optional>
#include "EventSubscription.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Models/Full/FullBenchmarkResultModel.hpp"
#include "Core/ThreadQueue.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class FullBenchmarkView;
	}

	class FullBenchmarkModel;
	class FullBenchmarkResultModel;
	class FullBenchmarkController;
	class BenchmarkRunConfigurationModel;
	class BenchmarkRunConfigurationController;

	class FullBenchmarkView : public QWidget
	{
	Q_OBJECT

	public:
		explicit FullBenchmarkView(
				const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
				BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
				const FullBenchmarkModel& model,
				FullBenchmarkController& controller,
				QWidget* parent = nullptr);

		~FullBenchmarkView() override;

	private:
		struct PreviousScores {
			Score _totalScore;
			Score _singleCoreScore;
			Score _multiCoreScore;
		};
		Ui::FullBenchmarkView* _ui;
		const FullBenchmarkModel& _model;
		FullBenchmarkController& _controller;

		EventSubscription<bool> _runningChanged;
		EventSubscription<const CollectionItem<FullBenchmarkResultModel>&> _itemAdded;
		EventSubscription<const std::string&> _currentBenchmarkChanged;
		EventSubscription<> _resultsCleared;
		std::optional<PreviousScores> _previousScores;
		std::weak_ptr<ThreadQueue> _threadQueue;
		int _currentBenchmarkIndex;
		int _maxBenchmarkIndex;
		bool _cancel;

		QString _na = "N/A";

		void UpdateProgress();
		void OnResultAdded(const FullBenchmarkResultModel& result);
		void OnResultsCleared();

	private slots:
		void on_bpStart_clicked(bool checked);
	};
}

#endif // BENCHMARKVIEW_HPP
