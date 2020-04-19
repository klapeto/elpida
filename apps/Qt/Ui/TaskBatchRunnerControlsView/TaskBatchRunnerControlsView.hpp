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
	class Mediator;

	class TaskBatchRunnerControlsView : public QWidget
	{
	Q_OBJECT

	public:
		explicit TaskBatchRunnerControlsView(Mediator& mediator, const TaskRunnerModel& model);
		~TaskBatchRunnerControlsView() override;
	private:
		Ui::TaskBatchRunnerControlsView* _ui;
		const TaskRunnerModel& _model;
		EventSubscriptionBase* _dataChangedEventSubscription;
		Mediator& _mediator;
		bool _running;
	signals:
		void onDataChanged();

	private slots:
		void updateUi();
		void startClicked(bool checked);
		void stopClicked(bool checked);
	};

} // namespace Elpida
#endif // ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP
