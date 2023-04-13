#ifndef BENCHMARKCONFIGURATIONVIEW_HPP
#define BENCHMARKCONFIGURATIONVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkConfigurationView;
	}

	class BenchmarkModel;

	class BenchmarkConfigurationView : public QWidget
	{
	 Q_OBJECT
	 public:
		void SetModel(BenchmarkModel* benchmarkModel);

		explicit BenchmarkConfigurationView();
		~BenchmarkConfigurationView() override;
	 private:
		Ui::BenchmarkConfigurationView* _ui;
		BenchmarkModel* _benchmarkModel;
		std::vector<QWidget*> _createdWidgets;
	};
}

#endif // BENCHMARKCONFIGURATIONVIEW_HPP
