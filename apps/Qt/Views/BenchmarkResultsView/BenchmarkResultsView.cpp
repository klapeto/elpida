#include "BenchmarkResultsView.hpp"
#include "ui_BenchmarkResultsView.h"

namespace Elpida::Application
{

	BenchmarkResultsView::BenchmarkResultsView(QWidget* parent) :
		QWidget(parent),
		_ui(new Ui::BenchmarkResultsView)
	{
		_ui->setupUi(this);
	}

	BenchmarkResultsView::~BenchmarkResultsView()
	{
		delete _ui;
	}

} // namespace Elpida
