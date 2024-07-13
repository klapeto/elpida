#ifndef BENCHMARKRUNNINGDIALOG_HPP
#define BENCHMARKRUNNINGDIALOG_HPP

#include <QDialog>

namespace Elpida::Application
{
	namespace Ui
	{
		class BenchmarkRunningDialog;
	}

	class BenchmarkRunningDialog : public QDialog
	{
	Q_OBJECT

	public:
		explicit BenchmarkRunningDialog(QWidget* parent = nullptr);
		~BenchmarkRunningDialog() override;
	private:
		Ui::BenchmarkRunningDialog* _ui;

	private slots:
		void on_btnCancel_clicked(bool checked);
	};
}

#endif // BENCHMARKRUNNINGDIALOG_HPP
