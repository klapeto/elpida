#ifndef BENCHMARKVIEW_HPP
#define BENCHMARKVIEW_HPP

#include <QWidget>
#include "EventSubscription.hpp"

namespace Elpida::Application
{
	namespace Ui
	{
		class FullBenchmarkView;
	}

	class FullBenchmarkModel;
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
		Ui::FullBenchmarkView* _ui;
		const FullBenchmarkModel& _model;
		FullBenchmarkController& _controller;

		EventSubscription<bool> _runningChanged;
		EventSubscription<const std::string&> _currentBenchmarkChanged;
		int _currentBenchmarkIndex;
		int _maxBenchmarkIndex;
		bool _cancel;

		void UpdateProgress();
		void UpdateScore();

	private slots:
		void on_bpStart_clicked(bool checked);
	};
}

#endif // BENCHMARKVIEW_HPP
