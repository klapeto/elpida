#include "BenchmarksView.hpp"
#include "ui_BenchmarksView.h"

namespace Elpida::Application
{
	BenchmarksView::BenchmarksView(BenchmarksModel& benchmarksModel)
		: QWidget(), _ui(new Ui::BenchmarksView), _benchmarksModel(benchmarksModel)
	{
		_ui->setupUi(this);
	}

	BenchmarksView::~BenchmarksView()
	{
		delete _ui;
	}
}