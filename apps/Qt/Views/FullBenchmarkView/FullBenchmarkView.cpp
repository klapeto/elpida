#include "FullBenchmarkView.hpp"
#include "ui_FullBenchmarkView.h"
#include "Views/BenchmarkRunConfigurationView/BenchmarkRunConfigurationView.hpp"

namespace Elpida::Application
{
	FullBenchmarkView::FullBenchmarkView(const BenchmarkRunConfigurationModel& benchmarkRunConfigurationModel,
			BenchmarkRunConfigurationController& benchmarkRunConfigurationController, QWidget* parent) :
			QWidget(parent),
			_ui(new Ui::FullBenchmarkView)
	{
		_ui->setupUi(this);
		dynamic_cast<QVBoxLayout*>(_ui->gbExecution->layout())->insertWidget(2, new BenchmarkRunConfigurationView(benchmarkRunConfigurationModel, benchmarkRunConfigurationController));
	}

	FullBenchmarkView::~FullBenchmarkView()
	{
		delete _ui;
	}
}