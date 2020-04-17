#ifndef ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP
#define ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP

#include <QWidget>

namespace Elpida
{

	namespace Ui
	{
		class TaskBatchRunnerControlsView;
	}

	class TaskRunnerModel;
	class EventSubscriptionBase;

	class TaskBatchRunnerControlsView : public QWidget
	{
	Q_OBJECT

	public:
		explicit TaskBatchRunnerControlsView(const TaskRunnerModel& model);
		~TaskBatchRunnerControlsView() override;
	private:
		Ui::TaskBatchRunnerControlsView* _ui;
		const TaskRunnerModel& _model;
		EventSubscriptionBase* _dataChangedEventSubscription;
		bool _running;

		void onDataChanged();
	};

} // namespace Elpida
#endif // ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP
