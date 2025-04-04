#include "BenchmarkRunningDialog.hpp"
#include "ui_BenchmarkRunningDialog.h"

#include "Controllers/CustomBenchmarkController.hpp"
#include <qnamespace.h>

namespace Elpida::Application
{
	BenchmarkRunningDialog::BenchmarkRunningDialog(QWidget* parent) :
		QDialog(parent),
		_ui(new Ui::BenchmarkRunningDialog)
	{
		_ui->setupUi(this);
	}

	BenchmarkRunningDialog::~BenchmarkRunningDialog()
	{
		delete _ui;
	}

	void BenchmarkRunningDialog::on_btnCancel_clicked(bool checked)
	{
		reject();
	}
}