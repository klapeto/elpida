#ifndef MAINWINDOW_HPP
#define MAINWINDOW_HPP

#include <QMainWindow>

namespace Ui
{
	class MainWindow;
}

class MainWindow final: public QMainWindow
{
	Q_OBJECT

	public:
		explicit MainWindow(QWidget *parent = 0);
		~MainWindow();

	protected:
		virtual void showEvent(QShowEvent *event) override;

	private slots:
		void on_actionExit_triggered();
		void on_actionAbout_triggered();

	private:
		Ui::MainWindow *_ui;
		bool _fixedSizeSet;

		void loadCpuInfo();
		void loadTaskInfo();
};

#endif // MAINWINDOW_HPP
