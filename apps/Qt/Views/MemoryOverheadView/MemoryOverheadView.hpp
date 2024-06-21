#ifndef MEMORYOVERHEADVIEW_HPP
#define MEMORYOVERHEADVIEW_HPP

#include <QWidget>
#include "EventSubscription.hpp"
#include "Models/MemoryOverhead/MemoryOverheadResultModel.hpp"
#include "Models/Abstractions/CollectionItem.hpp"
#include "Core/ThreadQueue.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class MemoryOverheadView;
	}

	class BenchmarkRunConfigurationModel;
	class BenchmarkRunConfigurationController;
	class BenchmarkRunConfigurationView;
	class MemoryOverheadCalculationController;
	class MemoryOverheadCalculationModel;

	class MemoryOverheadView : public QWidget
	{
	Q_OBJECT

	public:
		explicit MemoryOverheadView(
				const MemoryOverheadCalculationModel& model,
				MemoryOverheadCalculationController& controller,
				const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
				BenchmarkRunConfigurationController& benchmarkRunConfigurationController, QWidget* parent = nullptr);
		~MemoryOverheadView() override;

	private:
		Ui::MemoryOverheadView* _ui;
		BenchmarkRunConfigurationView* _runConfigurationView;
		MemoryOverheadCalculationController& _controller;
		const MemoryOverheadCalculationModel& _model;

		EventSubscription<bool> _runningChanged;
		EventSubscription<const CollectionItem<MemoryOverheadResultModel>&> _itemAdded;
		EventSubscription<const std::string&> _currentBenchmarkChanged;
		EventSubscription<> _resultsCleared;
		std::weak_ptr<ThreadQueue> _threadQueue;

		bool _updatingUi;

		int _currentBenchmarkIndex;
		int _maxBenchmarkIndex;

		void UpdateProgress();
		void OnResultAdded(const MemoryOverheadResultModel& result);
		void OnResultsCleared();

	private slots:
		void on_bpStart_clicked(bool checked);
		void on_spnInitialScale_valueChanged(double value);
		void on_spnInitialSubSamples_valueChanged(int value);
		void on_spnScaleMultiplier_valueChanged(double value);
		void on_spnSubSamplesMultiplier_valueChanged(double value);
		void on_spnIterations_valueChanged(int value);
	};
}

#endif // MEMORYOVERHEADVIEW_HPP
