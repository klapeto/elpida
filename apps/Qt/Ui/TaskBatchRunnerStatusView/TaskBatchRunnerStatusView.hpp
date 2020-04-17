#ifndef ELPIDA_TASKBATCHRUNNERSTATUS_H
#define ELPIDA_TASKBATCHRUNNERSTATUS_H

#include <QWidget>
#include "Models/TaskRunnerModel.hpp"

namespace Elpida
{

	class EventSubscriptionBase;
	class Task;
	class TaskBatch;

	namespace Ui
	{
		class TaskBatchRunnerStatusView;
	}

	class TaskBatchRunnerStatusView : public QWidget
	{
	Q_OBJECT

	public:
		explicit TaskBatchRunnerStatusView(const TaskRunnerModel& model);
		~TaskBatchRunnerStatusView();

	private:
		const TaskRunnerModel& _model;
		Ui::TaskBatchRunnerStatusView* _ui;
		EventSubscriptionBase* _dataChangedEventSubscription;
		const Task* _currentRunningTask;
		const TaskBatch* _currentRunningTaskBatch;

		QString _runningString;
		QString _readyString;
		QString _naString;

		void updateUi();
	};

} // namespace Elpida
#endif // ELPIDA_TASKBATCHRUNNERSTATUS_H
