#ifndef BENCHMARKVIEW_HPP
#define BENCHMARKVIEW_HPP

#include <QWidget>
namespace Elpida::Application
{
	namespace Ui
	{
		class FullBenchmarkView;
	}

	class BenchmarkRunConfigurationModel;
	class BenchmarkRunConfigurationController;

	class FullBenchmarkView : public QWidget
	{
	Q_OBJECT

	public:
		explicit FullBenchmarkView(
				const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
				BenchmarkRunConfigurationController& benchmarkRunConfigurationController,
				QWidget* parent = nullptr);

		~FullBenchmarkView() override;

	private:
		Ui::FullBenchmarkView* _ui;
	};
}

#endif // BENCHMARKVIEW_HPP
