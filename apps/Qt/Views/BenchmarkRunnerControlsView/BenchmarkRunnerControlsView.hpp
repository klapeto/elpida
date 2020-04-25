#ifndef ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP
#define ELPIDA_TASKBATCHRUNNERCONTROLSVIEW_HPP

#include <QWidget>

namespace Elpida
{

	namespace Ui
	{
		class BenchmarkRunnerControlsView;
	}

	class BenchmarkRunnerModel;
	class EventSubscriptionBase;
	class Mediator;

	class BenchmarkRunnerControlsView : public QWidget
	{
	Q_OBJECT

	public:
		explicit BenchmarkRunnerControlsView(Mediator& mediator, const BenchmarkRunnerModel& model);
		~BenchmarkRunnerControlsView() override;
	private:
		Ui::BenchmarkRunnerControlsView* _ui;
		const BenchmarkRunnerModel& _model;
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
