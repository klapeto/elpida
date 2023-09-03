#ifndef BENCHMARKRUNNINGDIALOG_HPP
#define BENCHMARKRUNNINGDIALOG_HPP

#include <QDialog>

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkRunningDialog;
	}

	class BenchmarksController;

	class BenchmarkRunningDialog : public QDialog
	{
	Q_OBJECT

	public:
		explicit BenchmarkRunningDialog(BenchmarksController& benchmarksController, QWidget* parent = nullptr);
		~BenchmarkRunningDialog() override;
	private:
		Ui::BenchmarkRunningDialog* _ui;
		BenchmarksController& _benchmarksController;

	private slots:
		void on_btnCancel_clicked(bool checked);
	};
}

#endif // BENCHMARKRUNNINGDIALOG_HPP
