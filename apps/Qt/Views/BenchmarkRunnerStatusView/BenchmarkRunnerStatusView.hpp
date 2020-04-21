#ifndef ELPIDA_TASKBATCHRUNNERSTATUS_H
#define ELPIDA_TASKBATCHRUNNERSTATUS_H

#include <QWidget>
#include "Models/BenchmarkRunnerModel.hpp"

namespace Elpida
{

	class EventSubscriptionBase;
	class TaskSpecification;
	class Benchmark;

	namespace Ui
	{
		class BenchmarkRunnerStatusView;
	}

	class BenchmarkRunnerStatusView : public QWidget
	{
	Q_OBJECT

	public:
		explicit BenchmarkRunnerStatusView(const BenchmarkRunnerModel& model);
		~BenchmarkRunnerStatusView() override;
	private:
		Ui::BenchmarkRunnerStatusView* _ui;
		const BenchmarkRunnerModel& _model;
		EventSubscriptionBase* _dataChangedEventSubscription;
		const TaskSpecification* _currentRunningTaskSpecification;
		const Benchmark* _currentRunningBenchmark;

		QString _runningString;
		QString _readyString;
		QString _naString;

		bool _running;

		signals:
		void onDataUpdated();

	private slots:
		void updateUi();
	};

} // namespace Elpida
#endif // ELPIDA_TASKBATCHRUNNERSTATUS_H
