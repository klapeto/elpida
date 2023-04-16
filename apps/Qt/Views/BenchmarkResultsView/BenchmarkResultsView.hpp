#ifndef ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
#define ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{

	namespace Ui
	{
		class BenchmarkResultsView;
	}

	class BenchmarkResultsView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit BenchmarkResultsView(QWidget* parent = nullptr);
		~BenchmarkResultsView() override;

	 private:
		Ui::BenchmarkResultsView* _ui;
	};

} // namespace Elpida
#endif // ELPIDA_APPLICATION_BENCHMARKRESULTSVIEW_HPP
