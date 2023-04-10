#ifndef BENCHMARKSVIEW_HPP
#define BENCHMARKSVIEW_HPP

#include <QWidget>

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarksView;
	}

	class BenchmarksModel;
	class BenchmarksView : public QWidget
	{
	 Q_OBJECT

	 public:
		explicit BenchmarksView(BenchmarksModel& benchmarksModel);
		~BenchmarksView() override;
	 private:
		Ui::BenchmarksView* _ui;
		BenchmarksModel& _benchmarksModel;
	};
}

#endif // BENCHMARKSVIEW_HPP
