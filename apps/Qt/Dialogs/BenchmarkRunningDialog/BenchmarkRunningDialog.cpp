#include "BenchmarkRunningDialog.hpp"
#include "ui_BenchmarkRunningDialog.h"

#include "Controllers/BenchmarksController.hpp"

namespace Elpida::Application
{
	BenchmarkRunningDialog::BenchmarkRunningDialog(BenchmarksController& benchmarksController, QWidget* parent) :
		QDialog(parent),
		_ui(new Ui::BenchmarkRunningDialog), _benchmarksController(benchmarksController)
	{
		_ui->setupUi(this);
	}

	BenchmarkRunningDialog::~BenchmarkRunningDialog()
	{
		delete _ui;
	}

	void BenchmarkRunningDialog::on_btnCancel_clicked(bool checked)
	{
		_benchmarksController.StopRunning();
		close();
	}

}