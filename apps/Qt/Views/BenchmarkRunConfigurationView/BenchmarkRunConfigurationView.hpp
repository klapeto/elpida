#ifndef BENCHMARKRUNCONFIGURATIONVIEW_HPP
#define BENCHMARKRUNCONFIGURATIONVIEW_HPP

#include <QWidget>
#include "EventSubscription.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkRunConfigurationView;
	}

	class BenchmarkRunConfigurationController;
	class BenchmarkRunConfigurationModel;

	class BenchmarkRunConfigurationView : public QWidget
	{
	Q_OBJECT

	public:

		void DisableAdvancedSettings();

		explicit BenchmarkRunConfigurationView(
				const BenchmarkRunConfigurationModel& model,
				BenchmarkRunConfigurationController& controller,
				QWidget* parent = nullptr);

		~BenchmarkRunConfigurationView() override;
	private:
		Ui::BenchmarkRunConfigurationView* _ui;
		const BenchmarkRunConfigurationModel& _model;
		BenchmarkRunConfigurationController& _controller;
		bool _uiUpdating;

		EventSubscription<> _modelChangedSubscription;
		void UpdateUi();
	private slots:

		void on_chkUpload_stateChanged(int state);

		void on_chkOpenResult_stateChanged(int state);

		void on_chkNumaAware_stateChanged(int state);

		void on_chkPinThreads_stateChanged(int state);

		void on_spnTimes_valueChanged(int value);

		void on_spnDependentRatio_valueChanged(double value);

		void on_spnIndependentRatio_valueChanged(double value);
	};
}
#endif // BENCHMARKRUNCONFIGURATIONVIEW_HPP
